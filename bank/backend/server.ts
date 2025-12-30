process.on("uncaughtException", (err) => {
  console.error("UNCAUGHT EXCEPTION:", err);
});

process.on("unhandledRejection", (reason) => {
  console.error("UNHANDLED REJECTION:", reason);
});
import express, { Request, Response } from 'express';
import bodyParser from 'body-parser';
import cors from 'cors';
import cron from 'node-cron';
import jwt from 'jsonwebtoken';
import pool from './db';

// Routes
import signupRoutes from './routes/signup';
import verifyRoutes from './routes/verify';
import loginRoutes from './routes/login';
import dashboardRoutes from './routes/dashboard';
import transferRoutes from './routes/transfer';

const PORT = 5000;
export const JWT_SECRET = 'my_secret_key';


const app = express();

// Middlewares
app.use(cors());
app.use(bodyParser.json());

// Routes
app.use('/signup', signupRoutes);
app.use('/verify-otp', verifyRoutes);
app.use('/login', loginRoutes);
app.use('/dashboard', dashboardRoutes);
app.use('/transfer', transferRoutes);

// Root
app.get('/', (_req: Request, res: Response) => {
  res.send('Server is running!');
});

// Cron job: clean unverified users older than 1 day
cron.schedule('0 0 * * *', async () => 
{
    try 
    {
    const result = await pool.query(
        `DELETE FROM users WHERE verified = false AND created_at < NOW() - INTERVAL '1 day'`
    );
    console.log(`${result.rowCount} unverified users were deleted`);
    } 
    catch (err) 
    {
    console.error('An error occurred cleaning up unverified users', err);
    }
});

// Start server
app.listen(PORT, '0.0.0.0', () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});
