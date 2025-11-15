"""
Comprehensive tests for user models and services.
"""
from datetime import timedelta
from django.test import TestCase
from django.utils import timezone
from django.contrib.auth import get_user_model
from django.core.files.uploadedfile import SimpleUploadedFile

from .models import Profile, Address, Verification
from .services.username_service import (
    generate_username_from_email,
    generate_username_from_name,
    make_unique_username,
    ensure_username
)
from .services.verification_service import (
    generate_otp,
    verify_otp_code,
    create_verification_for_user
)


User = get_user_model()


class UserModelTests(TestCase):
    """Tests for the User model."""

    def test_create_user_with_email(self):
        """Test creating a user with email generates username automatically."""
        user = User.objects.create_user(
            email='test@example.com',
            password='testpass123'
        )
        self.assertEqual(user.email, 'test@example.com')
        self.assertTrue(user.username)
        self.assertTrue(user.check_password('testpass123'))

    def test_create_user_with_phone(self):
        """Test creating a user with phone number."""
        user = User.objects.create_user(
            email='test2@example.com',
            password='testpass123',
            phone_number='+12025551234'
        )
        self.assertEqual(str(user.phone_number), '+12025551234')
        self.assertFalse(user.is_phone_verified)

    def test_user_string_representation(self):
        """Test user string representation."""
        user = User.objects.create_user(
            email='test@example.com',
            password='testpass123'
        )
        self.assertEqual(str(user), 'test@example.com')

    def test_user_full_name(self):
        """Test get_full_name method."""
        user = User.objects.create_user(
            email='test@example.com',
            password='testpass123',
            first_name='John',
            last_name='Doe'
        )
        self.assertEqual(user.get_full_name(), 'John Doe')


class ProfileModelTests(TestCase):
    """Tests for the Profile model."""

    def test_profile_auto_creation(self):
        """Test that profile is automatically created with user."""
        user = User.objects.create_user(
            email='test@example.com',
            password='testpass123'
        )
        self.assertTrue(hasattr(user, 'profile'))
        self.assertIsInstance(user.profile, Profile)

    def test_profile_string_representation(self):
        """Test profile string representation."""
        user = User.objects.create_user(
            email='test@example.com',
            password='testpass123',
            first_name='John',
            last_name='Doe'
        )
        self.assertIn('John Doe', str(user.profile))

    def test_profile_update(self):
        """Test updating profile fields."""
        user = User.objects.create_user(
            email='test@example.com',
            password='testpass123'
        )
        user.profile.bio = 'Test bio'
        user.profile.occupation = 'Software Engineer'
        user.profile.save()
        
        # Refresh from database
        user.refresh_from_db()
        self.assertEqual(user.profile.bio, 'Test bio')
        self.assertEqual(user.profile.occupation, 'Software Engineer')


class AddressModelTests(TestCase):
    """Tests for the Address model."""

    def setUp(self):
        """Set up test user."""
        self.user = User.objects.create_user(
            email='test@example.com',
            password='testpass123'
        )

    def test_create_address(self):
        """Test creating an address."""
        address = Address.objects.create(
            user=self.user,
            address_type='home',
            street_address_1='123 Main St',
            city='Anytown',
            state_province='CA',
            postal_code='12345',
            country='US'
        )
        self.assertEqual(address.user, self.user)
        self.assertEqual(address.city, 'Anytown')

    def test_default_address(self):
        """Test that only one default address per type exists."""
        address1 = Address.objects.create(
            user=self.user,
            address_type='home',
            street_address_1='123 Main St',
            city='Anytown',
            state_province='CA',
            postal_code='12345',
            country='US',
            is_default=True
        )
        
        address2 = Address.objects.create(
            user=self.user,
            address_type='home',
            street_address_1='456 Oak Ave',
            city='Other City',
            state_province='NY',
            postal_code='67890',
            country='US',
            is_default=True
        )
        
        # Refresh first address from database
        address1.refresh_from_db()
        
        # First address should no longer be default
        self.assertFalse(address1.is_default)
        self.assertTrue(address2.is_default)

    def test_formatted_address(self):
        """Test get_formatted_address method."""
        address = Address.objects.create(
            user=self.user,
            address_type='home',
            street_address_1='123 Main St',
            street_address_2='Apt 4',
            city='Anytown',
            state_province='CA',
            postal_code='12345',
            country='US'
        )
        formatted = address.get_formatted_address()
        self.assertIn('123 Main St', formatted)
        self.assertIn('Anytown', formatted)
        self.assertIn('CA', formatted)


class VerificationModelTests(TestCase):
    """Tests for the Verification model."""

    def setUp(self):
        """Set up test user."""
        self.user = User.objects.create_user(
            email='test@example.com',
            password='testpass123',
            phone_number='+12025551234'
        )

    def test_create_verification(self):
        """Test creating a verification record."""
        expires_at = timezone.now() + timedelta(minutes=10)
        verification = Verification.objects.create(
            user=self.user,
            verification_type='phone',
            phone_number=self.user.phone_number,
            otp_code='123456',
            expires_at=expires_at
        )
        self.assertEqual(verification.user, self.user)
        self.assertEqual(verification.otp_code, '123456')
        self.assertEqual(verification.status, 'pending')

    def test_is_valid_verification(self):
        """Test is_valid method."""
        expires_at = timezone.now() + timedelta(minutes=10)
        verification = Verification.objects.create(
            user=self.user,
            verification_type='phone',
            phone_number=self.user.phone_number,
            otp_code='123456',
            expires_at=expires_at
        )
        self.assertTrue(verification.is_valid())

    def test_expired_verification(self):
        """Test that expired verifications are not valid."""
        expires_at = timezone.now() - timedelta(minutes=10)
        verification = Verification.objects.create(
            user=self.user,
            verification_type='phone',
            phone_number=self.user.phone_number,
            otp_code='123456',
            expires_at=expires_at
        )
        self.assertFalse(verification.is_valid())

    def test_verify_correct_code(self):
        """Test verifying with correct code."""
        expires_at = timezone.now() + timedelta(minutes=10)
        verification = Verification.objects.create(
            user=self.user,
            verification_type='phone',
            phone_number=self.user.phone_number,
            otp_code='123456',
            expires_at=expires_at
        )
        result = verification.verify('123456')
        self.assertTrue(result)
        self.assertEqual(verification.status, 'verified')
        self.assertIsNotNone(verification.verified_at)

    def test_verify_incorrect_code(self):
        """Test verifying with incorrect code."""
        expires_at = timezone.now() + timedelta(minutes=10)
        verification = Verification.objects.create(
            user=self.user,
            verification_type='phone',
            phone_number=self.user.phone_number,
            otp_code='123456',
            expires_at=expires_at
        )
        result = verification.verify('654321')
        self.assertFalse(result)
        self.assertEqual(verification.status, 'pending')
        self.assertEqual(verification.attempts, 1)

    def test_max_attempts_exceeded(self):
        """Test that verification fails after max attempts."""
        expires_at = timezone.now() + timedelta(minutes=10)
        verification = Verification.objects.create(
            user=self.user,
            verification_type='phone',
            phone_number=self.user.phone_number,
            otp_code='123456',
            expires_at=expires_at,
            max_attempts=3
        )
        
        # Try wrong code 3 times
        verification.verify('111111')
        verification.verify('222222')
        verification.verify('333333')
        
        # Verification should be marked as failed
        self.assertEqual(verification.status, 'failed')
        self.assertEqual(verification.attempts, 3)


class UsernameServiceTests(TestCase):
    """Tests for username generation service."""

    def test_generate_username_from_email(self):
        """Test username generation from email."""
        username = generate_username_from_email('john.doe@example.com')
        self.assertEqual(username, 'johndoe')

    def test_generate_username_from_name(self):
        """Test username generation from name."""
        username = generate_username_from_name('John', 'Doe')
        self.assertEqual(username, 'john_doe')

    def test_make_unique_username(self):
        """Test making username unique."""
        # Create a user with a username
        User.objects.create_user(
            username='testuser',
            email='test1@example.com',
            password='testpass123'
        )
        
        # Try to create unique username with same base
        unique_username = make_unique_username('testuser')
        self.assertEqual(unique_username, 'testuser1')

    def test_ensure_username_from_email(self):
        """Test ensure_username with email."""
        username = ensure_username(email='test@example.com')
        self.assertIsNotNone(username)
        self.assertTrue(len(username) > 0)

    def test_ensure_username_from_name(self):
        """Test ensure_username with name."""
        username = ensure_username(first_name='John', last_name='Doe')
        self.assertIn('john', username.lower())


class VerificationServiceTests(TestCase):
    """Tests for verification service."""

    def test_generate_otp(self):
        """Test OTP generation."""
        otp = generate_otp()
        self.assertEqual(len(otp), 6)
        self.assertTrue(otp.isdigit())

    def test_verify_otp_code_correct(self):
        """Test OTP verification with correct code."""
        stored_code = '123456'
        provided_code = '123456'
        self.assertTrue(verify_otp_code(stored_code, provided_code))

    def test_verify_otp_code_incorrect(self):
        """Test OTP verification with incorrect code."""
        stored_code = '123456'
        provided_code = '654321'
        self.assertFalse(verify_otp_code(stored_code, provided_code))

    def test_create_verification_for_user(self):
        """Test creating verification for user."""
        user = User.objects.create_user(
            email='test@example.com',
            password='testpass123',
            phone_number='+12025551234'
        )
        
        verification = create_verification_for_user(
            user=user,
            verification_type='phone',
            phone_number=user.phone_number
        )
        
        self.assertIsNotNone(verification)
        self.assertEqual(verification.user, user)
        self.assertEqual(verification.verification_type, 'phone')
        self.assertTrue(len(verification.otp_code) > 0)
