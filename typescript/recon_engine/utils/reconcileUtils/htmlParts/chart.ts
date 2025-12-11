import type { Summary } from '../../types.js';

export function generateChartHtml(summary: Summary): string 
{
  return `
  <div style="width: 400px; height: 400px; margin: 20px auto;">
    <canvas id="reconciliationChart" width="400" height="400"></canvas>
  </div>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <script>
    const ctx = document.getElementById('reconciliationChart').getContext('2d');
    new Chart(ctx, {
      type: 'pie',
      data: {
        labels: ['Overpaid', 'Underpaid', 'Balanced'],
        datasets: [{
          data: [${summary.overpaid}, ${summary.underpaid}, ${summary.balanced}],
          backgroundColor: ['#e74c3c', '#f39c12', '#2ecc71'],
          borderColor: '#2c3e50',
          borderWidth: 1
        }]
      },
      options: {
        plugins: { 
          legend: { 
            position: 'bottom',
            labels: { color: '#ecf0f1', font: { size: 14 } } 
          } 
        },
        responsive: false,
        maintainAspectRatio: false
      }
    });
  </script>`;
}
