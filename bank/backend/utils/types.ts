
export interface User 
{
  id: number; // optional because DB generates it
  email: string;
  password: string;
  phone: string;
  balance: number;
  verified: boolean;
  otp_code: string | null;
  otp_expire_at: Date | null;
  created_at: Date;
}

export interface Transaction 
{
  id: number;
  sender_id: number;
  recipient_id: number;
  amount: number;
  createdAt: Date;
}
