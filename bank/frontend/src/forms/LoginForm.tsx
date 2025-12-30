import React from 'react';
import InputField from '../components/InputField';
import Button from '../components/Button';
import { useNavigate } from 'react-router-dom';

interface LoginFormProps {
  email: string;
  setEmail: (v: string) => void;
  password: string;
  setPassword: (v: string) => void;
  setError: (msg: string) => void;
  onSubmit: (e: React.FormEvent) => void;
}

const LoginForm: React.FC<LoginFormProps> = ({
  email,
  setEmail,
  password,
  setPassword,
  setError,
  onSubmit
}) => {
  const navigate = useNavigate();

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
        Login
      </h2>

      <form
        onSubmit={onSubmit}
        style={{
          display: 'flex',
          flexDirection: 'column',
          gap: '20px',
          width: '100%',
        }}
      >
        {/* EMAIL */}
        <InputField
          label="Email"
          value={email}
          onChange={(val) => {
            setEmail(val);
            setError('');
          }}
        />

        {/* PASSWORD */}
        <InputField
          label="Password"
          type="password"
          value={password}
          onChange={(val) => {
            setPassword(val);
            setError('');
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
          {/* LOGIN (submit) */}
          <Button text="Login" type="submit" />

          {/* SIGN UP (navigate only) */}
          <Button
            text="Sign Up"
            type="button"
            onClick={() => navigate('/signup')}
          />
        </div>
      </form>
    </>
  );
};

export default LoginForm;
