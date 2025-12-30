function generateOTP(): string {
  return String(Math.floor(100000 + Math.random() * 900000)); // 6 digits
}

export default generateOTP;
