
import path from 'path';
import fs from 'fs';

import type { Claim, Invoice } from '../utils/types.js';
import readCSV from '../utils/readCSV.js';
import groupInvoices from '../utils/groupInvoices.js';
import reconClaims from '../utils/reconClaims.js';
import summarize from '../utils/summarize.js';
import generateHTML from '../utils/generateHTML.js';

(async () => {
  const dataDir = path.join(process.cwd(), 'data');

  const claims = await readCSV<Claim>(path.join(dataDir, 'claims.csv'));
  const invoices = await readCSV<Invoice>(path.join(dataDir, 'invoices.csv'));

  //creating record of claim_id as keys and totals of related invoices
  const invoiceTotals = groupInvoices(invoices);

  //creating array of claims with reconciliation status
  const reconciled = reconClaims(claims, invoiceTotals);

  //creating summary section
  const summary = summarize(reconciled);

  const reportHTML = generateHTML(summary);

  const outputDir = path.join(process.cwd(), 'output');
  const outputPath = path.join(outputDir, 'reconciliation_report.html');

  fs.writeFileSync(outputPath, reportHTML);

  console.log(`Reconciliation complete! Report saved to ${outputPath}`);
})();
