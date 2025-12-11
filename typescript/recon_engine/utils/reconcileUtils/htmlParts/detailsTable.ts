import type { ReconciledClaim } from '../../types.js';

export function generateDetailsTableHtml(claims: ReconciledClaim[]): string 
{
  return `
  <h2>Detailed Claims</h2>
  <table>
    <tr>
      <th>Claim ID</th>
      <th>Patient ID</th>
      <th>Charges Amount</th>
      <th>Benefit Amount</th>
      <th>Total Transaction Value</th>
      <th>Status</th>
    </tr>
    ${claims.map(c => {
      const statusClass = c.reconciliation_status === 'BALANCED' ? 'balanced' :
                          c.reconciliation_status === 'OVERPAID' ? 'overpaid' : 'underpaid';
      return `
        <tr>
          <td>${c.claim_id}</td>
          <td>${c.patient_id}</td>
          <td>${Number(c.charges_amount).toFixed(2)}</td>
          <td>${Number(c.benefit_amount).toFixed(2)}</td>
          <td>${Number(c.total_transaction_value).toFixed(2)}</td>
          <td class="${statusClass}">${c.reconciliation_status}</td>
        </tr>`;
    }).join('')}
  </table>`;
}

