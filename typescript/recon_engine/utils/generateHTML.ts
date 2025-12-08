import type { Summary } from './types.js';
import { generateSummaryHtml } from './htmlParts/summarySection.js';
import { generateChartHtml } from './htmlParts/chart.js';
import { generateDetailsTableHtml } from './htmlParts/detailsTable.js';

export default function generateHtml(summary: Summary): string {
  return `
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8" />
<title>Reconciliation Report</title>
<style>
  body { font-family: Arial, sans-serif; padding: 20px; background: #2c3e50; color: #ecf0f1; }
  h1 { text-align: center; margin-bottom: 20px; }
  .summary-box, .summary-item { background: #34495e; border-radius: 10px; padding: 20px; margin-bottom: 20px; }
  .summary-grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 12px; }
  .label { font-weight: bold; }
  .balanced { color: #2ecc71; font-weight: bold; }
  .overpaid { color: #e74c3c; font-weight: bold; }
  .underpaid { color: #f39c12; font-weight: bold; }
  table { width: 100%; border-collapse: collapse; margin-top: 20px; background: #ecf0f1; color: #2c3e50; border-radius: 10px; overflow: hidden; }
  th { background: #34495e; color: white; padding: 10px; text-align: left; }
  td { border-bottom: 1px solid #ddd; padding: 8px; }
  tr:nth-child(even) { background: #f3f3f3; }
</style>
</head>
<body>
<h1>Claims Reconciliation Report</h1>
${generateSummaryHtml(summary)}
${generateChartHtml(summary)}
${generateDetailsTableHtml(summary.claims)}
</body>
</html>`;
}
