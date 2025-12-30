import React, { useEffect, useState } from "react";
import axios from "axios";
import DashboardForm from "../forms/DashboardForm";
import { getUserEmailFromToken } from '../utils/types';

interface Transaction {
  id: string | number;
  createdAt?: string;
  recipientEmail?: string;
  senderEmail?: string;
  type?: "sent" | "received";
  amount: number;
}

const Dashboard: React.FC = () => {
  const [balance, setBalance] = useState<number | null>(null);
  const [transactions, setTransactions] = useState<Transaction[]>([]);
  const [error, setError] = useState("");

  useEffect(() => {
    const fetchData = async () => {
      const token = localStorage.getItem("token");

      try {
        const res = await axios.get("http://localhost:5000/dashboard", {
          headers: { Authorization: `Bearer: ${token}` },
        });

        const userEmail = getUserEmailFromToken();

        const mappedTransactions: Transaction[] = res.data.transactions.map(
          (t: any) => ({
            id: t.id ?? Math.random(),
            createdAt: t.created_at,
            recipientEmail: t.recipient_email,
            senderEmail: t.sender_email,
            amount: Number(t.amount), // convert string to number
            type: t.sender_email === userEmail ? "sent" : "received",
          })
        );

        setBalance(res.data.balance);
        setTransactions(mappedTransactions);
      } catch (err: any) {
        console.error(err);
        setError(err.response?.data?.message || "Failed to fetch dashboard");
      }
    };

    fetchData();
  }, []);

  const handleTransferClick = () => {
    window.location.href = "/transfer";
  };

  return (
    <DashboardForm
      balance={balance}
      transactions={transactions}
      error={error}
      onTransferClick={handleTransferClick}
    />
  );
};

export default Dashboard;
