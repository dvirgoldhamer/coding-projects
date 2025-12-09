import { createObjectCsvWriter } from 'csv-writer';
import path from 'path';

export function writeCsv(filename: string, headers: any[], records: any[]) 
{
    const writer = createObjectCsvWriter({
    path: path.join(process.cwd(), 'data', filename),
    header: headers,
    });

    return writer.writeRecords(records);
}
