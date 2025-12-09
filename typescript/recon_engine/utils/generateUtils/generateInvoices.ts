import type { Invoice, Claim } from '../types.js';
import { randomInt, randomFloat } from '../random.js';
import { faker } from '@faker-js/faker';

export function generateInvoices(claims: Claim[]): Invoice[] 
{
    const invoices: Invoice[] = [];
    let counter = 1;

    for (const claim of claims) 
    {
        const numInvoices = randomInt(1, 5);

        for (let i = 0; i < numInvoices; i++) 
        {
            invoices.push({
                invoice_id: counter.toString(),
                claim_id: claim.claim_id,
                type_of_bill: faker.helpers.arrayElement(['fee', 'procedure payment']),
                transaction_value: randomFloat(-1000, 1000),
            });

            counter++;
        }
    }

    return invoices;
}
