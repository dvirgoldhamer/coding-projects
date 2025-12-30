import React from "react";

interface Transaction {
  id: string | number;
  createdAt?: string;
  recipientEmail?: string;
  senderEmail?: string;
  type?: "sent" | "received";
  amount: number;
}

interface DashboardFormProps {
  balance: number | null;
  transactions: Transaction[];
  error: string;
  onTransferClick: () => void;
}

const DashboardForm: React.FC<DashboardFormProps> = ({
  balance,
  transactions,
  error,
  onTransferClick,
}) => {
  return (
    <div
      style={{
        backgroundColor: "#2c2c3d",
        padding: "60px",
        borderRadius: "12px",
        boxShadow: "0 4px 15px rgba(0,0,0,0.3)",
        maxWidth: "700px",
        margin: "50px auto",
        color: "#f5f5f5",
      }}
    >
      <h2
        style={{
          textAlign: "center",
          fontSize: "32px",
          marginBottom: "30px",
        }}
      >
        My Account
      </h2>

      <p style={{ fontSize: "18px", marginBottom: "30px" }}>
        <b>Balance:</b> {balance !== null ? balance.toFixed(2) : "Loading..."}
      </p>

      <h3 style={{ fontSize: "20px", marginBottom: "15px" }}>Latest Transactions</h3>

      {transactions.length === 0 ? (
        <p>No recent transactions</p>
      ) : (
        <div style={{ overflowX: "auto", marginBottom: "30px" }}>
          <table style={{ width: "100%", borderCollapse: "collapse" }}>
            <thead>
              <tr>
                <th
                  style={{
                    textAlign: "left",
                    padding: "10px",
                    borderBottom: "2px solid #4e8cff",
                  }}
                >
                  Date
                </th>
                <th
                  style={{
                    textAlign: "left",
                    padding: "10px",
                    borderBottom: "2px solid #4e8cff",
                  }}
                >
                  Recipient
                </th>
                <th
                  style={{
                    textAlign: "left",
                    padding: "10px",
                    borderBottom: "2px solid #4e8cff",
                  }}
                >
                  Type
                </th>
                <th
                  style={{
                    textAlign: "right",
                    padding: "10px",
                    borderBottom: "2px solid #4e8cff",
                  }}
                >
                  Amount
                </th>
              </tr>
            </thead>
            <tbody>
              {transactions.map((t, idx) => (
                <tr
                  key={t.id}
                  style={{
                    borderBottom: "1px solid #444",
                    backgroundColor: idx % 2 === 0 ? "#333" : "#2c2c3d",
                  }}
                >
                  <td style={{ padding: "10px" }}>
                    {t.createdAt ? new Date(t.createdAt).toLocaleString() : "-"}
                  </td>
                  <td style={{ padding: "10px" }}>{t.recipientEmail || "-"}</td>
                  <td style={{ padding: "10px" }}>{t.type || "-"}</td>
                  <td style={{ padding: "10px", textAlign: "right" }}>
                    {typeof t.amount === "number" ? t.amount.toFixed(2) : "-"}
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      )}

      <div style={{ display: "flex", gap: "15px", justifyContent: "space-between" }}>
        <button
          onClick={onTransferClick}
          style={{
            flex: 1,
            padding: "12px",
            borderRadius: "6px",
            border: "none",
            backgroundColor: "#4e8cff",
            color: "#fff",
            fontSize: "16px",
            fontWeight: 600,
            cursor: "pointer",
          }}
        >
          Make a Transfer
        </button>
        <button
          onClick={() => (window.location.href = "/login")}
          style={{
            flex: 1,
            padding: "12px",
            borderRadius: "6px",
            border: "none",
            backgroundColor: "#ff6b6b",
            color: "#fff",
            fontSize: "16px",
            fontWeight: 600,
            cursor: "pointer",
          }}
        >
          Logout
        </button>
      </div>

      {error && (
        <p
          style={{
            color: "#ff6b6b",
            marginTop: "15px",
            textAlign: "center",
            fontWeight: 500,
          }}
        >
          {error}
        </p>
      )}
    </div>
  );
};

export default DashboardForm;
