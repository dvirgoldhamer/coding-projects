import express from 'express';
import type { Request, Response } from 'express';
import pool from '../db';
import bcrypt from 'bcryptjs';
import jwt from 'jsonwebtoken';

const JWT_SECRET = "my_secret_key";

const router = express.Router();

// Request body type
interface LoginRequestBody 
{
  email: string;
  password: string;
}

router.post('/', async (req: Request<{}, {}, LoginRequestBody>, res: Response) => 
{
    const { email, password } = req.body;

    if (!email || !password) 
    {
        return res.status(400).json({
            success: false,
            message: 'Invalid email or password',
        });
    }

  try 
  {
    const result = await pool.query<{ email: string; password: string; verified: boolean }>(
      `SELECT email, password, verified FROM users WHERE email = $1`,
      [email]
    );

    console.log("hello");
    
    if (result.rowCount === 0) {
        return res.status(400).json({
        success: false,
        message: 'Email not found',
        });
    }

    const user = result.rows[0]!;

    if (!user.verified) 
    {
        return res.status(403).json({
        success: false,
        message: 'Account not verified. Please Sign-up again and verify your acccount.',
        });
    }

    const match = await bcrypt.compare(password, user.password);

    if (!match) {
        return res.status(400).json({
        success: false,
        message: 'Incorrect password. Please try again',
        });
    }

    // Success: creating a JWT
    const token = jwt.sign({ email: user.email }, JWT_SECRET, { expiresIn: '1h' });

    return res.status(200).json({
        success: true,
        message: 'Successful Login',
        token: token
    });
  } 
  catch (err) 
  {
    console.error(err);
    return res.status(500).json({
        success: false,
        message: 'Internal server error',
    });
  }
});

export default router;
