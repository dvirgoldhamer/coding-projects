// utils/auth.ts
import { jwtDecode } from "jwt-decode";

interface JwtPayload {
  email: string;
  userId: string | number;
  exp: number;
}

export function getUserEmailFromToken(): string | null {
  const token = localStorage.getItem("token");
  if (!token) return null;

  try {
    const decoded = jwtDecode<JwtPayload>(token);
    return decoded.email;
  } catch {
    return null;
  }
}
