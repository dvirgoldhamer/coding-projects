

##sign-up
- POST /signup

- Request body
{
    "email": "user@example.com",
    "password": "MyPassword123",
    "phone": "0525345347"
}

- Response success
{
    "success": true,
    "messasge": "User created successfully. OTP was sent to your phone/email for verification"
}

- Response error
{
    "success": false,
    "message": "Email already exist. try a different Email" 
}

##login
- POST /login

-Request body
{
    "email": "user@example.com",
    "password": "MyPassword123"
}

- Response success
{
    "success": true,
    "token": "JWT_TOKEN_HERE"
}

- Response error
{
    "success": false,
    "message": "Invalid Email or Password"
}
