import React, { useState } from 'react';
import InputField from '../components/InputField';
import Button from '../components/Button';

interface TransferFormProps {
  onSubmit: (recipientEmail: string, amount: number) => void;
  setError: (msg: string) => void;
  setInfo: (msg: string) => void;
}

const TransferForm: React.FC<TransferFormProps> = ({ onSubmit, setError, setInfo }) => {
  const [recipientEmail, setRecipientEmail] = useState('');
  const [amount, setAmount] = useState('');

  const validate = (): boolean => {
    const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;

    if (!emailRegex.test(recipientEmail)) {
      setError('Invalid recipient email');
      return false;
    }

    if (Number(amount) <= 0 || isNaN(Number(amount))) {
      setError('Amount must be a positive number');
      return false;
    }

    setError('');
    return true;
  };

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    setInfo('');
    if (!validate()) return;
    onSubmit(recipientEmail, Number(amount));
  };

  return (
    <>
      <h2
        style={{
          color: '#f5f5f5',
          marginBottom: '20px',
          textAlign: 'center',
          fontSize: '32px',
        }}
      >
        Transfer Money
      </h2>

      <form
        onSubmit={handleSubmit}
        style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}
      >
        <InputField
          label="Recipient Email"
          value={recipientEmail}
          onChange={(val) => {
            setRecipientEmail(val);
            setError('');
            setInfo('');
          }}
        />

        <InputField
          label="Amount"
          type="number"
          value={amount}
          onChange={(val) => {
            setAmount(val);
            setError('');
            setInfo('');
          }}
        />

        <div
          style={{
            display: 'flex',
            gap: '15px',
            width: '100%',
            justifyContent: 'space-between',
          }}
        >
          <Button text="Send" type="submit" />
          <Button
            text="Return to Account"
            type="button"
            onClick={() => window.location.href = '/dashboard'}
          />
        </div>
      </form>
    </>
  );
};

export default TransferForm;
