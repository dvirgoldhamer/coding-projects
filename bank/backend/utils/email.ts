import nodemailer from 'nodemailer';

// Create transporter
const transporter = nodemailer.createTransport({
  service: 'Gmail',
  auth: {
    user: 'dvir.goldhamer@gmail.com',
    pass: 'fxgj txic lqij btvc', // Ideally use environment variables instead of hardcoding
  },
});

// Function to send OTP
async function sendOTPToEmail(send_to: string, otp_code: string): Promise<void> 
{

  const mailDetails: nodemailer.SendMailOptions = {
    from: 'dvir.goldhamer@gmail.com',
    to: send_to,
    subject: 'Your bank account OTP code',
    html: 
        `<p>your OTP code is <b>${otp_code}</b>. The code will be valid for the next 5 minutes.</p>`
  }

  try 
  {  
    await transporter.sendMail(mailDetails);
    console.log(`Verification email sent to ${send_to}`);
  } 
  catch (err) 
  {
    console.error('Error sending OTP code to email', err);
    throw err;
  }
}

export default sendOTPToEmail;
