import type { Request, Response, NextFunction } from 'express';
import type { User } from '../utils/types';
import jwt from 'jsonwebtoken';
import pool from '../db';

const JWT_SECRET = 'my_secret_key';

interface JwtPayload 
{
  email: string;
}

export async function AuthenticateJWT(req: Request & { user?: User },
  res: Response, next: NextFunction): Promise<void> 
{
  const authHeader = req.headers.authorization;

  if (!authHeader) {
    res.status(401).json({ success: false, message: 'Missing authorization header' });
    return;
  }

  const token = authHeader.split(' ')[1];

  try 
  {
    //checking the secret key, check expiration, returns decoded payload
    const decoded = jwt.verify(token, JWT_SECRET);

    if (!decoded || typeof decoded !== 'object' || !('email' in decoded)) 
    {
      res.status(401).json({ success: false, message: 'Invalid token payload' });
      return;
    }

    //just the email field
    const payload = decoded as JwtPayload;

    const result = await pool.query<User>(
      'SELECT * FROM users WHERE email = $1',
      [payload.email]
    );

    if (result.rowCount === 0) 
    {
      res.status(403).json({ success: false, message: 'User not in the bank' });
      return;
    }

    req.user = result.rows[0]; 

    next();
  } 
  catch (err) 
  {
    console.error(err);
    res.status(401).json({ success: false, message: 'Invalid or expired token' });
  }
}
