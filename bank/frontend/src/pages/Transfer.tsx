import React, { useState } from 'react';
import TransferForm from '../forms/TransferForm';
import { useNavigate } from 'react-router-dom';

const Transfer: React.FC = () => {
  const navigate = useNavigate();
  const [error, setError] = useState('');
  const [info, setInfo] = useState('');

  const handleTransfer = async (recipientEmail: string, amount: number) => {
    setError('');
    setInfo('');

    if (!recipientEmail || amount <= 0) {
      setError('Invalid input');
      return;
    }

    const token = localStorage.getItem('token');

    try {
      await fetch('http://localhost:5000/transfer', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          Authorization: `Bearer ${token}`,
        },
        body: JSON.stringify({ recipientEmail, amount }),
      });

      setInfo('Transaction completed successfully');
      setTimeout(() => navigate('/dashboard'), 2000);
    } catch (err: any) {
      console.error(err);
      setError(err.response?.data?.message || 'Unknown error');
    }
  };

  const formPadding = '60px';

  return (
    <div>
      <div
        style={{
          backgroundColor: '#2c2c3d',
          padding: formPadding,
          borderRadius: '12px',
          boxShadow: '0 4px 15px rgba(0,0,0,0.3)',
          width: '600px',
          margin: '0 auto',
        }}
      >
        <TransferForm
          onSubmit={handleTransfer}
          setError={setError}
          setInfo={setInfo}
        />

        {error && (
          <p
            style={{
              color: '#ff6b6b',
              marginTop: '15px',
              textAlign: 'center',
              fontWeight: 500,
            }}
          >
            {error}
          </p>
        )}

        {info && (
          <p
            style={{
              color: '#4e8cff',
              marginTop: '15px',
              textAlign: 'center',
              fontWeight: 500,
            }}
          >
            {info}
          </p>
        )}
      </div>
    </div>
  );
};

export default Transfer;
