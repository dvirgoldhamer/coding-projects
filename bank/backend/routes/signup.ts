import express from 'express';
import type { Request, Response } from 'express';
import pool from '../db';
import bcrypt from 'bcryptjs';
import { isValidEmail, isValidPassword, isValidPhone }  from '../utils/validate_info';

import generateOTP from '../utils/otp';
import sendOTPToEmail from '../utils/email';

const router = express.Router();

// Request body type
interface SignupRequestBody 
{
    name: string;
    email: string;
    password: string;
    phone: string;
}

router.post('/', async (req: Request<{}, {}, SignupRequestBody>, res: Response) => {
    const { name, email, password, phone } = req.body;
  
    // Validation
    if (!isValidEmail(email)) return res.status(400).json({ success: false, message: 'Invalid Email' });
    if (!isValidPassword(password)) return res.status(400).json({ success: false, message: 'Invalid Password' });
    if (!isValidPhone(phone)) return res.status(400).json({ success: false, message: 'Invalid Phone' });

    const hashedPassword = await bcrypt.hash(password, 10);
    const otpCode = generateOTP();
    const otpExpireAt = new Date(Date.now() + 5 * 60 * 1000);
    
    try 
    {
            console.log("hello");

        // Check if user exists
        const existing_email = await pool.query(`SELECT verified FROM users WHERE email=$1`, [email]);
        

    //there is this verified email in the database
    if (existing_email.rows.length > 0 && existing_email.rows[0].verified) 
    {
        return res.status(400).json({ success: false, message: 'Email already used. try a different one' });
    }

    // Insert new user
    const result = await pool.query(
      `INSERT INTO users(name, email, password, phone, otp_code, otp_expire_at)
       VALUES ($1, $2, $3, $4, $5, $6)
       RETURNING *`,
      [name, email, hashedPassword, phone, otpCode, otpExpireAt]
    );

    await sendOTPToEmail(email, otpCode);

    return res.status(201).json({
      success: true,
      message: 'User created. OTP code sent to your email for verification.',
    });

  } 
  catch (err: any) 
  {

    //unique constraint violation in db
    if (err.code === '23505') 
    {
        //there is this unverified email in the db  
        if (err.constraint === 'users_email_key') 
        {

            await pool.query(
            `UPDATE users
            SET name = $1, password = $2, phone = $3, otp_code = $4, otp_expire_at = $5
            WHERE email= $6`,
            [name, hashedPassword, phone, otpCode, otpExpireAt, email]);

            await sendOTPToEmail(email, otpCode);

            return res.status(200).json({
            success: true,
            message: 'User created. OTP code sent to your email for verification.',
            });
        } 
        else if (err.constraint === 'users_phone_key') 
        {
            return res.status(400).json({ success: false, message: 'Phone already used. try a different one' });
        }
    }

    return res.status(500).json({ success: false, message: 'Server error' });
  }
});

export default router;
