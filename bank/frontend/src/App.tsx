import React from 'react';
import { BrowserRouter, Routes, Route, Navigate } from 'react-router-dom';
import SignUp from './pages/SignUp';
import Login from './pages/Login';
import Dashboard from './pages/Dashboard';
import Transfer from './pages/Transfer';

function App() {
  return (
    <BrowserRouter>
      <div
        style={{
          minHeight: '100vh',
          backgroundImage: 'url(/images/bluestone_bg.jpg)', 
          backgroundSize: 'cover',       // makes image cover entire div
          backgroundPosition: 'center',  // centers the image
          backgroundRepeat: 'no-repeat', // prevents tiling
          display: 'flex',
          flexDirection: 'column',
          alignItems: 'center',
          padding: '20px',
        }}
      >
        <h1 style={{ color: '#130303ff', marginBottom: '80px', fontSize: '48px',
 }}>BlueStone Bank</h1>

        <Routes>
          <Route path="/" element={<Navigate to="/signup" replace />} />
          <Route path="/signup" element={<SignUp />} />
          <Route path="/login" element={<Login />} />
          <Route path="/dashboard" element={<Dashboard />} />
          <Route path="/transfer" element={<Transfer />} />
        </Routes>
      </div>
    </BrowserRouter>
  );
}

export default App;
