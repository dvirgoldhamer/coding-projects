import type { Summary } from './types.js';

export default function generateHTML(summary: Summary): string {
  return (`
  <html>
  <head>
    <title>Reconciliation Report</title>
    <style>
      body { font-family: Arial; padding: 20px; }
      table { width: 100%; border-collapse: collapse; margin-top: 20px; }
      th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }
      th { background: #f4f4f4; }
    </style>
  </head>
  <body>
    <h1>Reconciliation Summary</h1>
    <p>Total number of claims: ${summary.totalClaims}</p>
    <p>
    Balanced: ${summary.balanced} 
    (${((summary.balanced / summary.totalClaims) * 100).toFixed(2)}%)
    </p>
    <p>
    Overpaid: ${summary.overpaid} 
    (${((summary.overpaid / summary.totalClaims) * 100).toFixed(2)}%)
    </p>
    <p>
    Underpaid: ${summary.underpaid} 
    (${((summary.underpaid / summary.totalClaims) * 100).toFixed(2)}%)
    </p>
    <p>Total Overpaid Amount: ${summary.totalOverpaidAmount.toFixed(2)}</p>
    <p>Total Underpaid Amount: ${summary.totalUnderpaidAmount.toFixed(2)}</p>

    <h2>Detailed Claims</h2>
    <table>
      <tr>
        <th>claim_id</th>
        <th>patient_id</th>
        <th>charges_amount</th>
        <th>benefit_amount</th>
        <th>total_transaction_value</th>
        <th>status</th>
      </tr>
      ${summary.claims
        .map((c) => `
        <tr>
          <td>${c.claim_id}</td>
          <td>${c.patient_id}</td>
          <td>${c.charges_amount}</td>
          <td>${c.benefit_amount}</td>
          <td>${c.total_transaction_value}</td>
          <td>${c.reconciliation_status}</td>
        </tr>`
        )
        .join('')}
    </table>
  </body>
  </html>
  `);
}
