import React from 'react';

interface InputFieldProps {
  label: string;
  type?: string;
  value: string;
  onChange: (val: string) => void;
}

const InputField: React.FC<InputFieldProps> = ({ label, type = 'text', value, onChange }) => (
  <div style={{ marginBottom: '20px', display: 'flex', flexDirection: 'column' }}>
    <label style={{ marginBottom: '5px', fontWeight: 500, color: '#f5f5f5' }}>{label}</label>
    <input
      type={type}
      value={value}
      onChange={(e) => onChange(e.target.value)}
      style={{
        padding: '10px',
        borderRadius: '6px',
        border: '1px solid #555',
        backgroundColor: '#2c2c3d',
        color: '#f5f5f5',
        fontSize: '16px',
        outline: 'none',
      }}
      onFocus={(e) => (e.target.style.borderColor = '#4e8cff')}
      onBlur={(e) => (e.target.style.borderColor = '#555')}
    />
  </div>
);

export default InputField;
