
import express from 'express';
import type { Request, Response } from 'express';
import pool from '../db';
import { AuthenticateJWT } from '../middleware/jwt';
import type { User } from '../utils/types';

const router = express.Router();
const lastTransactionsNUM = 5;

router.get('/', AuthenticateJWT,  async (req: Request & { user?: User }, res: Response) => 
{
    try 
    {
        const result = await pool.query(
            `SELECT balance, id FROM users WHERE email=$1`,
            [req.user!.email]
        );

        const user_id = result.rows[0].id;

        const transactions = await pool.query(`
        SELECT 
            s.email AS sender_email,
            r.email AS recipient_email,
            t.amount,
            t.created_at
        FROM transactions t
        JOIN users s ON t.sender_id = s.id
        JOIN users r ON t.recipient_id = r.id
        WHERE t.sender_id = $1 OR t.recipient_id = $2
        ORDER BY t.id DESC
        LIMIT $3`, [user_id, user_id, lastTransactionsNUM]);

        res.status(200).json({
            success: true,
            balance: parseFloat(result.rows[0].balance),
            transactions: transactions.rows
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
