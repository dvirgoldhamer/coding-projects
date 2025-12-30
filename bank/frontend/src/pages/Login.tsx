import React, { useState } from 'react';
import axios from 'axios';
import LoginForm from '../forms/LoginForm';

const Login: React.FC = () => {
  const [email, setEmail] = useState('');
  const [infoMessage, setInfo] = useState('');
  const [password, setPassword] = useState('');
  const [error, setError] = useState('');

  const validate = (): boolean => {
    const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;

    if (!emailRegex.test(email)) {
      setError('Invalid email format');
      return false;
    }

    if (password.length < 6) {
      setError('Password must be at least 6 characters');
      return false;
    }

    return true;
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setError('');
    setInfo('');

    if (!validate()) {
      return;
    }

    try {
      const result = await axios.post('http://localhost:5000/login', {
        email,
        password,
      });

      const token = result.data.token;
      localStorage.setItem('token', token);

      setInfo('User verified. Redirecting to your account...');

      setTimeout(() => (window.location.href = '/dashboard'), 2000);
    } catch (err: any) {
      console.error(err.response?.data);
      setError(err.response?.data?.message || 'Login failed');
    }
  };

  const formPadding = '60px';

  return (
    <div>
      {/* Card container (same as SignUp) */}
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

        <LoginForm
          email={email}
          setEmail={setEmail}
          password={password}
          setPassword={setPassword}
          onSubmit={handleSubmit}
          setError={setError}
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

        {infoMessage && (
          <p
            style={{
              color: '#4e8cff',
              marginTop: '15px',
              textAlign: 'center',
              fontWeight: 500,
            }}
          >
            {infoMessage}
          </p>
        )}
      </div>
    </div>
  );
};

export default Login;
