
// Email validator
export function isValidEmail(email: string): boolean {
  const regex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
  return regex.test(email);
}

// Phone validator (exactly 10 digits)
export function isValidPhone(phone: string): boolean {
  const regex = /^\d{10}$/;
  return regex.test(phone);
}

// Password validator (at least 6 characters)
export function isValidPassword(password: string): boolean {
  return password.length >= 6;
}
