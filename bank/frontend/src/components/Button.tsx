import React from 'react';

interface ButtonProps {
  text: string;
  onClick?: () => void;
  type?: 'button' | 'submit';
}

const Button: React.FC<ButtonProps> = ({ text, onClick, type = 'button' }) => (
  <button
    type={type}
    onClick={onClick}
    style={{
      width: '45%',
      padding: '12px',
      borderRadius: '6px',
      border: 'none',
      backgroundColor: '#4e8cff',
      color: '#fff',
      fontSize: '16px',
      fontWeight: 600,
      cursor: 'pointer',
      transition: 'background-color 0.2s',
    }}
    onMouseEnter={(e) => ((e.target as HTMLButtonElement).style.backgroundColor = '#3b6edb')}
    onMouseLeave={(e) => ((e.target as HTMLButtonElement).style.backgroundColor = '#4e8cff')}
  >
    {text}
  </button>
);

export default Button;
