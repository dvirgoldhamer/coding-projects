import React, { useState } from 'react';
import axios from 'axios';
import InputField from '../components/InputField';
import Button from '../components/Button';
import { useNavigate } from 'react-router-dom';

interface SignUpFormParams {
  setError: (msg: string) => void;
  setInfo: (msg: string) => void;
  onSuccess: (email: string) => void;
}

const SignUpForm: React.FC<SignUpFormParams> = ({ setError, setInfo, onSuccess }) => {
    const navigate = useNavigate();
  
    const [name, setName] = useState('');
    const [email, setEmail] = useState('');
    const [password, setPassword] = useState('');
    const [phone, setPhone] = useState('');

    const validate = (): boolean => {
    const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    const phoneRegex = /^\d{10}$/;

    if (!emailRegex.test(email)) {
      setError('Invalid email format');
      return false;
    }
    if (password.length < 6) {
      setError('Password must be at least 6 characters');
      return false;
    }
    if (!phoneRegex.test(phone)) {
      setError('Invalid phone number');
      return false;
    }
    setError('');
    return true;
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setInfo('');
    setError('');

    if (!validate()) return;

    try 
    {
        const response = await axios.post('http://localhost:5000/signup', {
        name, email, password, phone,
        });
        setInfo(response.data.message);
        onSuccess(email);
    } 
    catch (err: any)
    {
      console.error(err);
      const message = err.response?.data?.message || err.message || 'Unknown error';
      setError(message);
    }
  };

  return (
    <>
      <h2 style={{ color: '#f5f5f5', marginBottom: '20px', textAlign: 'center', fontSize: '32px' }}>
        Sign Up
      </h2>

      <form
        onSubmit={handleSubmit}
        style={{
          display: 'flex',
          flexDirection: 'column',
          gap: '20px',
          width: '100%',
        }}
      >
        <InputField
          label="Full Name"
          value={name}
          onChange={(val) => {
            setName(val);
            setError('');
            setInfo('');
          }}
        />

        <InputField
          label="Email"
          value={email}
          onChange={(val) => {
            setEmail(val);
            setError('');
            setInfo('');
          }}
        />

        <InputField
          label="Password"
          type="password"
          value={password}
          onChange={(val) => {
            setPassword(val);
            setError('');
            setInfo('');
          }}
        />

        <InputField
          label="Phone"
          value={phone}
          onChange={(val) => {
            setPhone(val);
            setError('');
            setInfo('');
          }}
        />

        {/* --- BUTTONS ROW --- */}
        <div
          style={{
            display: 'flex',
            gap: '15px',
            width: '100%',
            justifyContent: 'space-between',
          }}
        >
          <Button text="Sign Up" type="submit" />

          {/* Login button navigates only */}
          <Button
            text="Login"
            type="button"
            onClick={() => navigate('/login')}
          />
        </div>
      </form>
    </>
  );
};

export default SignUpForm;
