import React, { useState } from 'react';
import SignUpForm from '../forms/SignUpForm';
import OtpForm from '../forms/OtpForm';

const SignUp: React.FC = () => {
  const [infoMessage, setInfo] = useState('');
  const [error, setError] = useState('');
  const [showOtpInput, setShowOtpInput] = useState(false);
  const [emailForOtp, setEmailForOtp] = useState('');

  const handleSignUpSuccess = (email: string) => {
    setEmailForOtp(email);
    setShowOtpInput(true);
    setInfo('');
    setError('');
  };
  const formPadding = '60px';    // <--- padding inside the frame

  return (
    <div>
      {/* Shared form card */}
      <div
        style={{
          backgroundColor: '#2c2c3d',
          padding: formPadding,
          borderRadius: '12px',
          boxShadow: '0 4px 15px rgba(0,0,0,0.3)',
          width: '600px'
        }}
      >
        {!showOtpInput ? (
          <SignUpForm
            onSuccess={handleSignUpSuccess}
            setError={setError}
            setInfo={setInfo}
          />
        ) : (
          <OtpForm
            email={emailForOtp}
            setError={setError}
            setInfo={setInfo}
          />
        )}

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

export default SignUp;
