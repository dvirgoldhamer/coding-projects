import type { Claim, Patient } from '../types.js';
import { randomInt, randomFloat } from '../random.js';
import { faker } from '@faker-js/faker';

export function generateClaims(patients: Patient[]): Claim[] 
{
    const claims: Claim[] = [];
    let counter = 1;

    for (const patient of patients) 
    {
        const numClaims = randomInt(2, 20);

        for (let i = 0; i < numClaims; i++) 
        {
            const charges = randomFloat(1, 1000);
            const benefit = randomFloat(0, charges);

            claims.push({
            claim_id: counter.toString(),
            patient_id: patient.patient_id,
            date_of_service: faker.date.past({ years: 2 })!.toISOString().split('T')[0],
            charges_amount: charges,
            benefit_amount: benefit,
            cpt_code: randomInt(10000, 99999).toString(),
            modifier: randomInt(1, 99).toString(),
            });

            counter++;
        }
    }

  return claims;
}
