"""
Verification service for SMS/OTP handling.

Handles OTP generation, validation, and SMS sending.
"""
import secrets
import string
from datetime import timedelta
from django.utils import timezone
from django.conf import settings


# OTP configuration
OTP_LENGTH = 6
OTP_EXPIRY_MINUTES = 10
OTP_MAX_ATTEMPTS = 3


def generate_otp(length=OTP_LENGTH):
    """
    Generate a secure random OTP code.
    
    Args:
        length: Length of the OTP code
        
    Returns:
        str: Random numeric OTP code
    """
    digits = string.digits
    otp = ''.join(secrets.choice(digits) for _ in range(length))
    return otp


def verify_otp_code(stored_code, provided_code):
    """
    Verify an OTP code securely using constant-time comparison.
    
    Args:
        stored_code: The stored OTP code
        provided_code: The code provided by the user
        
    Returns:
        bool: True if codes match, False otherwise
    """
    if not stored_code or not provided_code:
        return False
    
    # Use constant-time comparison to prevent timing attacks
    return secrets.compare_digest(str(stored_code), str(provided_code))


def send_sms_verification(phone_number, otp_code):
    """
    Send an SMS verification code to a phone number.
    
    This is a stub implementation. In production, integrate with a real
    SMS service provider like Twilio, AWS SNS, or similar.
    
    Args:
        phone_number: Phone number to send SMS to
        otp_code: The OTP code to send
        
    Returns:
        tuple: (success: bool, error_message: str or None)
    """
    try:
        # TODO: Integrate with actual SMS provider
        # Example with Twilio:
        # from twilio.rest import Client
        # client = Client(settings.TWILIO_ACCOUNT_SID, settings.TWILIO_AUTH_TOKEN)
        # message = client.messages.create(
        #     body=f"Your verification code is: {otp_code}",
        #     from_=settings.TWILIO_PHONE_NUMBER,
        #     to=str(phone_number)
        # )
        
        # For now, just log it (in production, this would actually send)
        print(f"SMS to {phone_number}: Your verification code is {otp_code}")
        
        return True, None
        
    except Exception as e:
        error_message = f"Failed to send SMS: {str(e)}"
        print(error_message)
        return False, error_message


def create_verification_for_user(user, verification_type='phone', phone_number=None, email=None):
    """
    Create a new verification record for a user.
    
    Args:
        user: User instance to create verification for
        verification_type: Type of verification ('phone', 'email', 'two_factor')
        phone_number: Phone number for phone verification
        email: Email for email verification
        
    Returns:
        Verification: Created verification instance
    """
    from ..models import Verification
    
    # Generate OTP
    otp_code = generate_otp()
    
    # Calculate expiry time
    expires_at = timezone.now() + timedelta(minutes=OTP_EXPIRY_MINUTES)
    
    # Create verification record
    verification = Verification.objects.create(
        user=user,
        verification_type=verification_type,
        phone_number=phone_number or user.phone_number,
        email=email or user.email,
        otp_code=otp_code,
        expires_at=expires_at,
        max_attempts=OTP_MAX_ATTEMPTS
    )
    
    # Send the code based on verification type
    if verification_type == 'phone' and verification.phone_number:
        send_sms_verification(verification.phone_number, otp_code)
    elif verification_type == 'email' and verification.email:
        # TODO: Implement email sending
        print(f"Email to {verification.email}: Your verification code is {otp_code}")
    
    return verification


def cleanup_expired_verifications():
    """
    Clean up expired verification records.
    
    This should be run periodically (e.g., via a cron job or Celery task).
    """
    from ..models import Verification
    
    expired_count = Verification.objects.filter(
        expires_at__lt=timezone.now(),
        status='pending'
    ).update(status='expired')
    
    return expired_count
