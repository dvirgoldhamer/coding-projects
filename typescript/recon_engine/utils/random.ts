export const randomInt = (min: number, max: number) =>
  Math.floor(Math.random() * (max - min) + min);

export const randomFloat = (min: number, max: number, decimals = 2) =>
  parseFloat((Math.random() * (max - min) + min).toFixed(decimals));
