import fs from 'fs';
import path from 'path';

import { generatePatients } from '../utils/generateUtils/generatePatients.js';
import { generateClaims } from '../utils/generateUtils/generateClaims.js';
import { generateInvoices } from '../utils/generateUtils/generateInvoices.js';
import { writeCsv } from '../utils/generateUtils/writeCsv.js';

const NUM_PATIENTS = 200;

(async () => 
{
  const dataDir = path.join(process.cwd(), 'data');
  if (!fs.existsSync(dataDir)) fs.mkdirSync(dataDir);

  const patients = generatePatients(NUM_PATIENTS);
  const claims = generateClaims(patients);
  const invoices = generateInvoices(claims);

  await writeCsv('patients.csv', [
    { id: 'patient_id', title: 'patient_id' },
    { id: 'name', title: 'name' }
  ], patients);

  await writeCsv('claims.csv', [
    { id: 'claim_id', title: 'claim_id' },
    { id: 'patient_id', title: 'patient_id' },
    { id: 'date_of_service', title: 'date_of_service' },
    { id: 'charges_amount', title: 'charges_amount' },
    { id: 'benefit_amount', title: 'benefit_amount' },
    { id: 'cpt_code', title: 'cpt_code' },
    { id: 'modifier', title: 'modifier' },
  ], claims);

  await writeCsv('invoices.csv', [
    { id: 'invoice_id', title: 'invoice_id' },
    { id: 'claim_id', title: 'claim_id' },
    { id: 'type_of_bill', title: 'type_of_bill' },
    { id: 'transaction_value', title: 'transaction_value' }
  ], invoices);

  console.log('Data generated successfully');
})();
