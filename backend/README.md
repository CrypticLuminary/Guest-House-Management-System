# Guest House Management System - Django Backend

This is the Django backend for the Guest House Management System. It provides user authentication, profile management, and related services.

## Features

### User Management
- Custom User model extending Django's AbstractUser
- Automatic username generation from email or name
- Phone number verification via SMS/OTP
- User avatars with automatic image compression

### Profile Management
- Extended user profiles with additional information
- ID proof tracking
- Emergency contact information
- User preferences stored as JSON

### Address Management
- Multiple addresses per user
- Support for different address types (home, work, billing, shipping)
- Default address management
- International address support with country field

### Verification System
- SMS/OTP verification for phone numbers
- Configurable expiry times and max attempts
- Secure OTP generation and validation
- Automatic verification status tracking

## Architecture

### Separation of Concerns

The codebase follows best practices with clear separation:

1. **Models** (`models.py`): Data models only, minimal business logic
2. **Services** (`services/`): Business logic extracted from models
   - `username_service.py`: Username generation logic
   - `image_service.py`: Image compression and validation
   - `verification_service.py`: OTP generation and SMS sending
3. **Signals** (`signals.py`): Event handlers for model lifecycle
4. **Admin** (`admin.py`): Django admin configuration

### Key Design Decisions

- **No broad try-except blocks**: Specific exception handling for different failure scenarios
- **Robust field access**: Signal handlers check for field existence before use
- **Clean imports**: No duplicate or unused imports
- **Proper docstrings**: All classes and methods documented
- **Service layer**: Business logic separated from models for testability

## Installation

1. Install dependencies:
```bash
pip install -r requirements.txt
```

2. Run migrations:
```bash
python manage.py makemigrations
python manage.py migrate
```

3. Create a superuser:
```bash
python manage.py createsuperuser
```

4. Run the development server:
```bash
python manage.py runserver
```

## Running Tests

Run all tests:
```bash
python manage.py test users
```

Run specific test class:
```bash
python manage.py test users.tests.UserModelTests
```

Run with verbose output:
```bash
python manage.py test users --verbosity=2
```

## Models Overview

### User Model
- Extends `AbstractUser`
- Fields: email (unique), phone_number, avatar, is_phone_verified, date_of_birth
- Automatic username generation on save
- Avatar compression on save

### Profile Model
- One-to-one with User
- Fields: bio, occupation, company, website, id_proof_type, id_proof_number, emergency contacts, preferences
- Automatically created when User is created (via signals)

### Address Model
- Foreign key to User (one user can have multiple addresses)
- Fields: address_type, street addresses, city, state/province, postal_code, country, is_default
- Only one default address per address type per user

### Verification Model
- Foreign key to User
- Fields: verification_type, phone_number, email, otp_code, status, attempts, expires_at
- Methods for checking validity and verifying codes

## Signal Handlers

### User post_save
- Creates Profile automatically when User is created
- Saves Profile when User is saved

### Profile post_save
- Creates phone verification when profile has unverified phone number
- Sends SMS with OTP code
- Specific error handling for validation, network, and other errors

### Verification post_save
- Marks user's phone as verified when verification is completed

## Services

### Username Service
- `generate_username_from_email()`: Extract username from email
- `generate_username_from_name()`: Generate from first/last name
- `make_unique_username()`: Ensure uniqueness by appending numbers
- `ensure_username()`: Try multiple strategies to generate valid username

### Image Service
- `compress_image()`: Resize and compress images
- `compress_user_avatar()`: Specifically handle user avatars
- `validate_image_file()`: Validate image files for security

### Verification Service
- `generate_otp()`: Generate secure random OTP
- `verify_otp_code()`: Constant-time comparison for security
- `send_sms_verification()`: Send SMS (stub, needs SMS provider integration)
- `create_verification_for_user()`: Create verification and send code
- `cleanup_expired_verifications()`: Clean up old verifications (for cron job)

## Integration Notes

### SMS Provider
The `send_sms_verification()` function in `verification_service.py` is a stub. To integrate with a real SMS provider:

1. Install provider SDK (e.g., Twilio):
```bash
pip install twilio
```

2. Add credentials to settings:
```python
TWILIO_ACCOUNT_SID = 'your_account_sid'
TWILIO_AUTH_TOKEN = 'your_auth_token'
TWILIO_PHONE_NUMBER = 'your_twilio_number'
```

3. Update the function to use the provider's API

## Security Considerations

- OTP codes use `secrets` module for cryptographically secure randomness
- OTP verification uses constant-time comparison to prevent timing attacks
- Image uploads are validated and compressed
- File extension validation on avatar uploads
- Max file size enforcement (5MB)
- Proper indexing on frequently queried fields

## Future Enhancements

- Email verification workflow
- Two-factor authentication
- Social authentication (OAuth)
- Rate limiting for verification attempts
- Celery tasks for async operations (SMS sending, image processing)
- Redis caching for verification codes
