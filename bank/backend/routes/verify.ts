import express from 'express';
import type { Request, Response } from 'express';
import pool from '../db';

const router = express.Router();

// request body type
interface VerifyOtpRequestBody 
{
  email: string;
  otpCode: string;
}

// DB user row type
interface UserRow 
{
  verified: boolean;
  otp_code: string | null;
  otp_expire_at: Date | null;
}

router.post('/', async (req: Request<{}, {}, VerifyOtpRequestBody>, res: Response) => 
{
    const { email, otpCode } = req.body;

    if (!email || !otpCode) 
    {
        console.log(`email is ${email}, otp is ${otpCode}`);
        return res.status(400).json({
        success: false,
        message: 'Invalid email or OTP code',
        });
    }

    try 
    {
        const result = await pool.query<UserRow>(
        `SELECT verified, otp_code, otp_expire_at FROM users WHERE email = $1`,
        [email]
        );

        if (result.rowCount === 0) 
        {
            return res.status(400).json({
                success: false,
                message: 'Email not found',
            });
        }

        const user = result.rows[0]!;

        if (user.otp_code !== otpCode) 
        {
            return res.status(400).json({
                success: false,
                message: 'Invalid OTP code',
            });
        }

        if (user.verified) 
        {
            return res.status(400).json({
                success: false,
                message: 'User already verified',
            });
        }

        if (!user.otp_expire_at || Date.now() > new Date(user.otp_expire_at).getTime()) 
        {
            return res.status(400).json({
                success: false,
                message: 'OTP expired',
            });
        }

        await pool.query(
        `UPDATE users SET verified = true, otp_code = NULL, otp_expire_at = NULL WHERE email = $1`,
        [email]
        );

        return res.status(200).json({
        success: true,
        message: 'OTP verified successfully',
        });
    } 
    catch (err) 
    {
        console.error(err);
        return res.status(500).json({
        success: false,
        message: 'Server error',
        });
    }
  }
);

export default router;
