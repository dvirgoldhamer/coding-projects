import { createObjectCsvWriter } from 'csv-writer';
import type { Patient, Claim, Invoice } from '../utils/types.js';
import path from 'path';
import fs from 'fs';
import { randomInt, randomFloat } from '../utils/random.js';

const NUM_PATIENTS = 200;

(async () => 
{
    const { faker } = await import('@faker-js/faker');

    const dataDir = path.join(process.cwd(), 'data');
    if (!fs.existsSync(dataDir))
    {
        fs.mkdirSync(dataDir);  
    } 

    //generating patients
    const patients: Patient[] = [];
    for (let i = 1; i <= NUM_PATIENTS; i++) 
    {
        patients.push({ patient_id: i.toString(), name: faker.person.fullName() });
    }

    //generating claims
    const claims: Claim[] = [];
    let claimCounter = 1;
    for (const patient of patients) 
    {
        const numClaims = randomInt(2, 20);
        for (let i = 0; i < numClaims; i++) 
        {
            const charges = randomFloat(1, 1000);
            const benefit = randomFloat(0, charges);
            const cpt_code = randomInt(10000,99999).toString();
            const modifier = randomInt(1,99).toString();

            claims.push({
                claim_id: claimCounter.toString(),
                patient_id: patient.patient_id,
                date_of_service: faker.date.past({ years: 2 })!.toISOString().split('T')[0],
                charges_amount: charges,
                benefit_amount: benefit,
                cpt_code: cpt_code,
                modifier: modifier
            });
            claimCounter++;
        }
    }

    //generating claims
    const invoices: Invoice[] = [];
    let invoiceCounter = 1;

    for (const claim of claims) 
    {
        const numInvoices = randomInt(1, 5);
        for (let i = 0; i < numInvoices; i++) 
        {
            invoices.push({
            invoice_id: invoiceCounter.toString(),
            claim_id: claim.claim_id,
            type_of_bill: faker.helpers.arrayElement(['fee', 'procedure payment']),
            transaction_value: randomFloat(-1000, 1000)
            });
            invoiceCounter++;
        }
    } 

  const patientsWriter = createObjectCsvWriter({
    path: path.join(dataDir, 'patients.csv'),
    header: [{ id: 'patient_id', title: 'patient_id' },
             { id: 'name', title: 'name' }]
  });

  const claimsWriter = createObjectCsvWriter({
    path: path.join(dataDir, 'claims.csv'),
    header: [{ id: 'claim_id', title: 'claim_id' },
            { id: 'patient_id', title: 'patient_id' },
            { id: 'date_of_service', title: 'date_of_service' },
            { id: 'charges_amount', title: 'charges_amount' },
            { id: 'benefit_amount', title: 'benefit_amount' },
            { id: 'cpt_code', title: 'cpt_code'},
            { id: 'modifier', title: 'modifier'}]
  });

  const invoicesWriter = createObjectCsvWriter({
    path: path.join(dataDir, 'invoices.csv'),
    header: [{ id: 'invoice_id', title: 'invoice_id' },
            { id: 'claim_id', title: 'claim_id' },
            { id: 'type_of_bill', title: 'type_of_bill' },
            { id: 'transaction_value', title: 'transaction_value' }]
  });

  await patientsWriter.writeRecords(patients);
  await claimsWriter.writeRecords(claims);
  await invoicesWriter.writeRecords(invoices);

  console.log('Fake data generated successfully in data/ folder!');
})();
