"""
User authentication and profile models for the Guest House Management System.

This module contains the core user-related models with proper separation of concerns:
- User: Custom user model for authentication
- Profile: Extended user information and preferences
- Address: User address information
- Verification: SMS/OTP verification tracking
"""
from django.contrib.auth.models import AbstractUser, UserManager as BaseUserManager
from django.core.validators import FileExtensionValidator
from django.db import models
from django.utils.translation import gettext_lazy as _
from phonenumber_field.modelfields import PhoneNumberField
from django_countries.fields import CountryField


class UserManager(BaseUserManager):
    """
    Custom user manager for User model.
    
    Allows creating users without requiring username, which will be
    auto-generated from email or name.
    """
    
    def create_user(self, email=None, password=None, **extra_fields):
        """
        Create and save a regular user with the given email and password.
        
        Username is generated automatically if not provided.
        """
        if not email:
            raise ValueError('The Email field must be set')
        
        email = self.normalize_email(email)
        
        # Username will be generated in the model's save method
        extra_fields.setdefault('username', '')
        
        user = self.model(email=email, **extra_fields)
        user.set_password(password)
        user.save(using=self._db)
        return user
    
    def create_superuser(self, email=None, password=None, **extra_fields):
        """
        Create and save a superuser with the given email and password.
        """
        extra_fields.setdefault('is_staff', True)
        extra_fields.setdefault('is_superuser', True)
        extra_fields.setdefault('is_active', True)

        if extra_fields.get('is_staff') is not True:
            raise ValueError('Superuser must have is_staff=True.')
        if extra_fields.get('is_superuser') is not True:
            raise ValueError('Superuser must have is_superuser=True.')

        return self.create_user(email, password, **extra_fields)


class User(AbstractUser):
    """
    Custom user model extending Django's AbstractUser.
    
    This model handles authentication and basic user information.
    Username auto-generation and image handling are delegated to services.
    """
    
    objects = UserManager()
    
    email = models.EmailField(
        _('email address'),
        unique=True,
        help_text=_('Required. A valid email address.')
    )
    phone_number = PhoneNumberField(
        _('phone number'),
        blank=True,
        null=True,
        help_text=_('Contact phone number in international format')
    )
    avatar = models.ImageField(
        _('avatar'),
        upload_to='avatars/%Y/%m/%d/',
        blank=True,
        null=True,
        validators=[
            FileExtensionValidator(
                allowed_extensions=['jpg', 'jpeg', 'png', 'gif', 'webp']
            )
        ],
        help_text=_('Profile picture (max 5MB, formats: jpg, png, gif, webp)')
    )
    is_phone_verified = models.BooleanField(
        _('phone verified'),
        default=False,
        help_text=_('Designates whether the user has verified their phone number.')
    )
    date_of_birth = models.DateField(
        _('date of birth'),
        blank=True,
        null=True
    )
    created_at = models.DateTimeField(
        _('created at'),
        auto_now_add=True
    )
    updated_at = models.DateTimeField(
        _('updated at'),
        auto_now=True
    )

    class Meta:
        verbose_name = _('user')
        verbose_name_plural = _('users')
        ordering = ['-created_at']
        indexes = [
            models.Index(fields=['email']),
            models.Index(fields=['phone_number']),
        ]

    def __str__(self):
        """Return string representation of the user."""
        return self.email or self.username

    def save(self, *args, **kwargs):
        """
        Save the user instance.
        
        Delegates username generation and image compression to service layer
        to keep model logic clean and testable.
        """
        from .services.username_service import ensure_username
        from .services.image_service import compress_user_avatar
        
        # Ensure username is set before saving
        if not self.username:
            self.username = ensure_username(self.email, self.first_name, self.last_name)
        
        # Handle avatar compression before saving
        if self.avatar and hasattr(self.avatar, 'file'):
            compress_user_avatar(self.avatar)
        
        super().save(*args, **kwargs)

    def get_full_name(self):
        """Return the user's full name."""
        full_name = f"{self.first_name} {self.last_name}".strip()
        return full_name or self.username


class Profile(models.Model):
    """
    Extended user profile information.
    
    This model stores additional user information that is not required
    for authentication. It has a one-to-one relationship with User.
    """
    user = models.OneToOneField(
        User,
        on_delete=models.CASCADE,
        related_name='profile',
        primary_key=True,
        verbose_name=_('user')
    )
    bio = models.TextField(
        _('biography'),
        blank=True,
        max_length=500,
        help_text=_('Brief description about yourself (max 500 characters)')
    )
    occupation = models.CharField(
        _('occupation'),
        max_length=100,
        blank=True
    )
    company = models.CharField(
        _('company'),
        max_length=100,
        blank=True
    )
    website = models.URLField(
        _('website'),
        blank=True
    )
    id_proof_type = models.CharField(
        _('ID proof type'),
        max_length=50,
        blank=True,
        choices=[
            ('passport', _('Passport')),
            ('national_id', _('National ID')),
            ('drivers_license', _('Driver\'s License')),
            ('other', _('Other')),
        ]
    )
    id_proof_number = models.CharField(
        _('ID proof number'),
        max_length=100,
        blank=True
    )
    emergency_contact_name = models.CharField(
        _('emergency contact name'),
        max_length=100,
        blank=True
    )
    emergency_contact_phone = PhoneNumberField(
        _('emergency contact phone'),
        blank=True,
        null=True
    )
    preferences = models.JSONField(
        _('preferences'),
        default=dict,
        blank=True,
        help_text=_('User preferences stored as JSON')
    )
    created_at = models.DateTimeField(
        _('created at'),
        auto_now_add=True
    )
    updated_at = models.DateTimeField(
        _('updated at'),
        auto_now=True
    )

    class Meta:
        verbose_name = _('profile')
        verbose_name_plural = _('profiles')

    def __str__(self):
        """Return string representation of the profile."""
        return f"Profile for {self.user.get_full_name()}"


class Address(models.Model):
    """
    User address information.
    
    Supports multiple addresses per user for billing and shipping purposes.
    """
    ADDRESS_TYPES = [
        ('home', _('Home')),
        ('work', _('Work')),
        ('billing', _('Billing')),
        ('shipping', _('Shipping')),
        ('other', _('Other')),
    ]

    user = models.ForeignKey(
        User,
        on_delete=models.CASCADE,
        related_name='addresses',
        verbose_name=_('user')
    )
    address_type = models.CharField(
        _('address type'),
        max_length=20,
        choices=ADDRESS_TYPES,
        default='home'
    )
    street_address_1 = models.CharField(
        _('street address line 1'),
        max_length=255
    )
    street_address_2 = models.CharField(
        _('street address line 2'),
        max_length=255,
        blank=True
    )
    city = models.CharField(
        _('city'),
        max_length=100
    )
    state_province = models.CharField(
        _('state/province'),
        max_length=100
    )
    postal_code = models.CharField(
        _('postal code'),
        max_length=20
    )
    country = CountryField(
        _('country'),
        default='US'
    )
    is_default = models.BooleanField(
        _('is default'),
        default=False,
        help_text=_('Designates if this is the default address for this user.')
    )
    created_at = models.DateTimeField(
        _('created at'),
        auto_now_add=True
    )
    updated_at = models.DateTimeField(
        _('updated at'),
        auto_now=True
    )

    class Meta:
        verbose_name = _('address')
        verbose_name_plural = _('addresses')
        ordering = ['-is_default', '-created_at']
        indexes = [
            models.Index(fields=['user', 'is_default']),
        ]

    def __str__(self):
        """Return string representation of the address."""
        return f"{self.get_address_type_display()} - {self.street_address_1}, {self.city}"

    def save(self, *args, **kwargs):
        """
        Save the address instance.
        
        Ensures only one default address per user and address type.
        """
        if self.is_default:
            # Set all other addresses of the same type for this user to non-default
            Address.objects.filter(
                user=self.user,
                address_type=self.address_type,
                is_default=True
            ).exclude(pk=self.pk).update(is_default=False)
        
        super().save(*args, **kwargs)

    def get_formatted_address(self):
        """Return a formatted address string."""
        address_parts = [
            self.street_address_1,
            self.street_address_2,
            self.city,
            self.state_province,
            self.postal_code,
            str(self.country.name)
        ]
        return ', '.join(filter(None, address_parts))


class Verification(models.Model):
    """
    SMS/OTP verification tracking for user phone numbers.
    
    This model stores verification attempts and status for phone number
    verification. OTP generation and SMS sending are handled by services.
    """
    VERIFICATION_TYPES = [
        ('phone', _('Phone Verification')),
        ('email', _('Email Verification')),
        ('two_factor', _('Two-Factor Authentication')),
    ]

    STATUS_CHOICES = [
        ('pending', _('Pending')),
        ('verified', _('Verified')),
        ('expired', _('Expired')),
        ('failed', _('Failed')),
    ]

    user = models.ForeignKey(
        User,
        on_delete=models.CASCADE,
        related_name='verifications',
        verbose_name=_('user')
    )
    verification_type = models.CharField(
        _('verification type'),
        max_length=20,
        choices=VERIFICATION_TYPES,
        default='phone'
    )
    phone_number = PhoneNumberField(
        _('phone number'),
        blank=True,
        null=True,
        help_text=_('Phone number being verified')
    )
    email = models.EmailField(
        _('email address'),
        blank=True,
        help_text=_('Email address being verified')
    )
    otp_code = models.CharField(
        _('OTP code'),
        max_length=10,
        blank=True,
        help_text=_('One-time password code')
    )
    status = models.CharField(
        _('status'),
        max_length=20,
        choices=STATUS_CHOICES,
        default='pending'
    )
    attempts = models.PositiveIntegerField(
        _('verification attempts'),
        default=0,
        help_text=_('Number of verification attempts')
    )
    max_attempts = models.PositiveIntegerField(
        _('maximum attempts'),
        default=3,
        help_text=_('Maximum allowed verification attempts')
    )
    expires_at = models.DateTimeField(
        _('expires at'),
        help_text=_('When this verification code expires')
    )
    verified_at = models.DateTimeField(
        _('verified at'),
        null=True,
        blank=True,
        help_text=_('When the verification was completed')
    )
    created_at = models.DateTimeField(
        _('created at'),
        auto_now_add=True
    )
    updated_at = models.DateTimeField(
        _('updated at'),
        auto_now=True
    )

    class Meta:
        verbose_name = _('verification')
        verbose_name_plural = _('verifications')
        ordering = ['-created_at']
        indexes = [
            models.Index(fields=['user', 'status']),
            models.Index(fields=['phone_number', 'status']),
            models.Index(fields=['expires_at']),
        ]

    def __str__(self):
        """Return string representation of the verification."""
        return f"{self.get_verification_type_display()} for {self.user.email} - {self.status}"

    def is_valid(self):
        """Check if the verification is still valid."""
        from django.utils import timezone
        return (
            self.status == 'pending' and
            self.expires_at > timezone.now() and
            self.attempts < self.max_attempts
        )

    def verify(self, code):
        """
        Verify the provided code against this verification.
        
        Args:
            code: The verification code to check
            
        Returns:
            bool: True if verification succeeded, False otherwise
        """
        from django.utils import timezone
        from .services.verification_service import verify_otp_code
        
        if not self.is_valid():
            return False
        
        self.attempts += 1
        
        if verify_otp_code(self.otp_code, code):
            self.status = 'verified'
            self.verified_at = timezone.now()
            self.save()
            return True
        
        if self.attempts >= self.max_attempts:
            self.status = 'failed'
        
        self.save()
        return False
