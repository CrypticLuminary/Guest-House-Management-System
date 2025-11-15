"""
Signal handlers for user-related models.

This module contains signal handlers for automatic profile creation
and verification management with proper error handling and field access.
"""
from django.db.models.signals import post_save
from django.dispatch import receiver
from django.utils import timezone
import logging

from .models import User, Profile, Verification


logger = logging.getLogger(__name__)


@receiver(post_save, sender=User)
def create_user_profile(sender, instance, created, **kwargs):
    """
    Automatically create a Profile instance when a new User is created.
    
    Args:
        sender: The model class (User)
        instance: The actual instance being saved
        created: Boolean indicating if this is a new instance
        **kwargs: Additional keyword arguments
    """
    if created:
        try:
            Profile.objects.create(user=instance)
            logger.info(f"Profile created for user: {instance.email}")
        except Exception as e:
            logger.error(f"Failed to create profile for user {instance.email}: {e}")


@receiver(post_save, sender=User)
def save_user_profile(sender, instance, **kwargs):
    """
    Save the user's profile when the user is saved.
    
    This ensures the profile stays in sync with the user model.
    
    Args:
        sender: The model class (User)
        instance: The actual instance being saved
        **kwargs: Additional keyword arguments
    """
    try:
        if hasattr(instance, 'profile'):
            instance.profile.save()
    except Profile.DoesNotExist:
        # Profile might not exist if user was created before signal was connected
        logger.warning(f"Profile does not exist for user {instance.email}, creating one")
        Profile.objects.create(user=instance)
    except Exception as e:
        logger.error(f"Failed to save profile for user {instance.email}: {e}")


@receiver(post_save, sender=Profile)
def handle_phone_verification(sender, instance, created, **kwargs):
    """
    Handle phone number verification when a profile is created or phone number changes.
    
    Creates a verification record and sends SMS if phone number is present.
    Uses specific exception handling for different failure scenarios.
    
    Args:
        sender: The model class (Profile)
        instance: The actual profile instance being saved
        created: Boolean indicating if this is a new instance
        **kwargs: Additional keyword arguments
    """
    # Get the user associated with this profile
    user = instance.user
    
    # Check if user has a phone number that needs verification
    if not user.phone_number:
        return
    
    # Check if phone is already verified
    if user.is_phone_verified:
        return
    
    # Check if there's already a pending verification for this phone number
    existing_verification = Verification.objects.filter(
        user=user,
        phone_number=user.phone_number,
        verification_type='phone',
        status='pending',
        expires_at__gt=timezone.now()
    ).first()
    
    if existing_verification:
        logger.info(f"Pending verification already exists for {user.phone_number}")
        return
    
    # Create a new verification
    try:
        from .services.verification_service import create_verification_for_user
        
        verification = create_verification_for_user(
            user=user,
            verification_type='phone',
            phone_number=user.phone_number
        )
        
        logger.info(
            f"Phone verification created for user {user.email}, "
            f"phone: {user.phone_number}, OTP: {verification.otp_code}"
        )
        
    except ValueError as e:
        # Handle validation errors (e.g., invalid phone number format)
        logger.error(
            f"Invalid phone number format for user {user.email}: {e}"
        )
    except ConnectionError as e:
        # Handle network/SMS service errors
        logger.error(
            f"Failed to send SMS to {user.phone_number} for user {user.email}: {e}"
        )
    except Exception as e:
        # Handle any other unexpected errors
        logger.error(
            f"Unexpected error creating verification for user {user.email}: {e}",
            exc_info=True
        )


@receiver(post_save, sender=Verification)
def mark_user_phone_verified(sender, instance, **kwargs):
    """
    Mark user's phone as verified when a phone verification is completed.
    
    Args:
        sender: The model class (Verification)
        instance: The actual verification instance
        **kwargs: Additional keyword arguments
    """
    if (
        instance.verification_type == 'phone' and
        instance.status == 'verified' and
        not instance.user.is_phone_verified
    ):
        try:
            instance.user.is_phone_verified = True
            instance.user.save(update_fields=['is_phone_verified'])
            logger.info(f"Marked phone as verified for user: {instance.user.email}")
        except Exception as e:
            logger.error(
                f"Failed to mark phone as verified for user {instance.user.email}: {e}"
            )
