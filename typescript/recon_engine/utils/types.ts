export interface Patient 
{
  patient_id: string;
  name: string;
}

export interface Claim 
{
  claim_id: string;
  patient_id: string;
  date_of_service: string;
  charges_amount: number;
  benefit_amount: number;
  cpt_code: string;
  modifier: string;
}

export interface Invoice 
{
  invoice_id: string;
  claim_id: string;
  type_of_bill: 'fee' | 'procedure payment';
  transaction_value: number;
}

export interface ReconciledClaim extends Claim 
{
  total_transaction_value: number;
  difference: number;
  reconciliation_status: ReconciliationStatus;
}

export interface Summary 
{
  claims: ReconciledClaim[];
  totalClaims: number;
  balanced: number;
  overpaid: number;
  underpaid: number;
  totalOverpaidAmount: number;
  totalUnderpaidAmount: number;
}

export type ReconciliationStatus = 'BALANCED' | 'OVERPAID' | 'UNDERPAID';

