import React, { useState } from "react";
import axios from "axios";
import InputField from "../components/InputField";
import LongButton from "../components/LongButton";

interface OtpFormParams {
  email: string;
  setError: (msg: string) => void;
  setInfo: (msg: string) => void;
}

const OtpForm: React.FC<OtpFormParams> = ({ email, setError, setInfo }) => {
  const [otp, setOtp] = useState("");

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setError("");
    setInfo("");

    try {
      await axios.post("http://localhost:5000/verify-otp", {
        email,
        otpCode: otp,
      });

      setInfo("OTP code verified. Redirecting to your account...");
      setTimeout(() => (window.location.href = "/dashboard"), 2000);
    } catch (err: any) {
      console.error(err);
      setError(err.response?.data?.message || "Failed to verify OTP");
    }
  };

  return (
    <>
      <h2
        style={{
          color: "#f5f5f5",
          marginBottom: "20px",
          textAlign: "center",
          fontSize: "32px",
        }}
      >
        OTP Verification
      </h2>

      <form
        onSubmit={handleSubmit}
        style={{
          display: "flex",
          flexDirection: "column",
          gap: "20px",
          width: "100%",
        }}
      >
        {/* OTP FIELD */}
        <InputField
          label="OTP Code"
          value={otp}
          onChange={(val) => {
            setOtp(val);
            setError("");
            setInfo("");
          }}
        />
        <div
          style={{
            display: "flex",
            gap: "15px",
            width: "100%",
            justifyContent: "space-between",
          }}
        >
          <LongButton text="Verify OTP" type="submit" />

        </div>
      </form>
    </>
  );
};

export default OtpForm;
