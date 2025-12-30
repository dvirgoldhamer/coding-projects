import express from 'express';
import type { Request, Response } from 'express';
import type { User } from '../utils/types';
import pool from '../db';
import { AuthenticateJWT } from '../middleware/jwt';

const router = express.Router();

interface UserRow 
{
  id: number;
  email: string;
  balance: number;
}

interface TransactionRow 
{
  sender_id: number;
  recipient_id: number;
  amount: number;
}

// Request body type
interface TransferRequestBody 
{
  recipientEmail: string;
  amount: number;
}

router.post('/', AuthenticateJWT, async (req: Request<{}, {}, TransferRequestBody> & { user?: User }, res: Response) => 
{
    const { recipientEmail, amount } = req.body;

    const senderEmail = req.user?.email;

    // Validate amount 
    const numericAmount = Number(amount);
    if (Number.isNaN(numericAmount) || numericAmount <= 0) 
    {
        return res.status(400).json({
        success: false,
        message: 'Invalid transfer amount',
      });
    }

    const client = await pool.connect();

    try 
    {
        const recipientResult = await client.query<UserRow>(
        `SELECT id, email, balance FROM users WHERE email = $1`,
        [recipientEmail]
        );

        if (recipientResult.rowCount === 0) 
        {
            return res.status(400).json({
                success: false,
                message: 'Recipient not found',
            });
        }

        const recipient = recipientResult.rows[0]!;

        const senderResult = await client.query<UserRow>(
        `SELECT id, email, balance FROM users WHERE email = $1`,
        [senderEmail]
        );

        const sender = senderResult.rows[0]!;

        //prevent self money transfer
        if (sender.email === recipient.email) 
        {
            return res.status(400).json({
                success: false,
                message: 'Cannot send money to yourself',
            });
        }

        // Check sender balance
        if (sender.balance < numericAmount) {
        return res.status(400).json({
            success: false,
            message: 'Insufficient balance',
        });
        }

        await client.query('BEGIN');

        // Update recipient balance
        await client.query(
        `UPDATE users SET balance = balance + $1 WHERE email = $2`,
        [numericAmount, recipientEmail]
        );

        // Update sender balance
        await client.query(
        `UPDATE users SET balance = balance - $1 WHERE email = $2`,
        [numericAmount, senderEmail]
        );

        // Log transaction
        await client.query<TransactionRow>(
        `INSERT INTO transactions (sender_id, recipient_id, amount) VALUES ($1, $2, $3)`,
        [sender.id, recipient.id, numericAmount]
        );

        await client.query('COMMIT');

        return res.status(200).json({
        success: true,
        message: 'Money successfully transferred',
        });
    }
    catch (err) 
    {
      console.error(err);
      await client.query('ROLLBACK');

        return res.status(500).json({
        success: false,
        message: 'Internal server error',
        });
    } 
    finally 
    {
      client.release();
    }
  }
);

export default router;
