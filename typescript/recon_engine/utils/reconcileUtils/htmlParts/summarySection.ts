import type { Summary } from '../types.js';

export function generateSummaryHtml(summary: Summary): string {
  return `
  <div class="summary-box">
    <h2>Summary</h2>
    <div class="summary-grid">
      <div class="summary-item"><span class="label">Total Claims:</span> ${summary.totalClaims}</div>
      <div class="summary-item"><span class="label">Balanced:</span> ${summary.balanced} (${((summary.balanced/summary.totalClaims)*100).toFixed(2)}%)</div>
      <div class="summary-item"><span class="label">Overpaid:</span> ${summary.overpaid} (${((summary.overpaid/summary.totalClaims)*100).toFixed(2)}%)</div>
      <div class="summary-item"><span class="label">Underpaid:</span> ${summary.underpaid} (${((summary.underpaid/summary.totalClaims)*100).toFixed(2)}%)</div>
      <div class="summary-item"><span class="label">Total Overpaid Amount:</span> ${summary.totalOverpaidAmount.toFixed(2)}</div>
      <div class="summary-item"><span class="label">Total Underpaid Amount:</span> ${summary.totalUnderpaidAmount.toFixed(2)}</div>
    </div>
  </div>`;
}

