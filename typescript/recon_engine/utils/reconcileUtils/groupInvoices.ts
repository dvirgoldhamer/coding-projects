import type { Invoice } from '../types.js';

//key is claim_id, value is total_transactions_value for claim
export default function groupInvoices(invoices: Invoice[]): Record<string, number> 
{
  const totals: Record<string, number> = {};

    for (const invoice of invoices) 
    {
        const transaction_value = Number(invoice.transaction_value);

        totals[invoice.claim_id] = (totals[invoice.claim_id] || 0) + transaction_value;

        totals[invoice.claim_id] = Number(totals[invoice.claim_id].toFixed(2));
    }

  return totals;
}
