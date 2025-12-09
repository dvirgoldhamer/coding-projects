import type { Patient } from '../types.js';
import { faker } from '@faker-js/faker';

export function generatePatients(num: number): Patient[] 
{
    const patients: Patient[] = [];

    for (let i = 1; i <= num; i++) 
    {
    patients.push({
        patient_id: i.toString(),
        name: faker.person.fullName(),
    });
    }

  return patients;
}
