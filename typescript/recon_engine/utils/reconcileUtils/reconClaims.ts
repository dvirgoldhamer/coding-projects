import type { Claim, ReconciledClaim, ReconciliationStatus } from './types.js';

export default function reconClaims(claims: Claim[], totals: Record<string, number>): ReconciledClaim[] 
{
    const reconciled: ReconciledClaim[] = [];

    for (const claim of claims) 
    {
        const total = totals[claim.claim_id] || 0;

        const reconciledClaim: ReconciledClaim = 
        {
            // copy all fields from claim
            ...claim,
            total_transaction_value: total,
            difference: Math.abs(total - claim.benefit_amount),  
            reconciliation_status: total > claim.benefit_amount ? "OVERPAID"
            : total === claim.benefit_amount ? "BALANCED" 
            : "UNDERPAID"
        };

        reconciled.push(reconciledClaim);
    }
    
    return reconciled;
}

