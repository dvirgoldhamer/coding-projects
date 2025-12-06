import { ReconciledClaim, Summary } from "./types.js";

export default function summarize(reconciled: ReconciledClaim[]): Summary
{
    //counting balanced claims
    const countBalanced = reconciled.filter(c => c.reconciliation_status === 'BALANCED').length;

    //counting overpaid claims and summing the difference
    const { countOverpaid, sumOverpaid } = reconciled.reduce((acc, c) => 
    {
        if (c.reconciliation_status === 'OVERPAID') 
        {
        acc.countOverpaid++;
        acc.sumOverpaid += (c.difference || 0);
        }

        return acc;
    }, 
    { countOverpaid: 0, sumOverpaid: 0 });

    //counting underpaid claims and summing the difference
    const { countUnderpaid, sumUnderpaid } = reconciled.reduce((acc, c) => 
    {
        if (c.reconciliation_status === 'UNDERPAID') 
        {
        acc.countUnderpaid++;
        acc.sumUnderpaid += (c.difference || 0);
        }

        return acc;
    }, 
    { countUnderpaid: 0, sumUnderpaid: 0 });

    return {
        claims: reconciled,
        totalClaims: reconciled.length,
        balanced: countBalanced,
        overpaid: countOverpaid,
        underpaid: countUnderpaid,
        totalOverpaidAmount: sumOverpaid,
        totalUnderpaidAmount: sumUnderpaid
    };
}   