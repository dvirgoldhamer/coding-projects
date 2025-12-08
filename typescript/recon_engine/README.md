
# Reconciliation Engine

## Project Overview

This project is a Reconciliation Engine built in TypeScript. It is doing three main things 

- Generates fake data for patients, claims, and invoices as CSV files.
- Reads the generated CSV files to reconcile claims status against their respective invoices.
- Produces an HTML report which summarizes claim reconciliation, and shows all of the claims in a detailed table.

## Project Launching

- all of the files inside the recon_engine directory (root dir) must be downloaded - and put in the same project tree.
- run 'npm install'.
- run 'npm run generate' in the root dir (to generate fake data file inside the 'data' directory).
- run 'npm run reconcile to generate the HTML report (which will be created in the output directory).

## Project output

The HTML report includes a summary section - describing the amount and percentage of underpaid, balanced and overpaid
claims (a balanced claim for example would be a claim where the insurance company paid the exact amount of paid invoices by 
the client).
The report also includes a table which shows all of the claims, with the respective fields - claim_id, patient_id, 
charges_amount, benefit_amoun, total_transaction_value and reconciliation status.





