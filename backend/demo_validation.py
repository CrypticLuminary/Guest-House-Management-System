#!/usr/bin/env python
"""
Demo validation script to showcase the refactored models functionality.

This script demonstrates:
1. User creation with auto-generated username
2. Automatic profile creation
3. Address management
4. Phone verification workflow
"""
import os
import sys
import django

# Setup Django
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'config.settings')
django.setup()

from users.models import User, Profile, Address, Verification
from users.services.verification_service import create_verification_for_user
from django.utils import timezone


def demo():
    """Run a demonstration of the models."""
    print("=" * 70)
    print("DJANGO MODELS REFACTORING DEMO")
    print("=" * 70)
    
    # 1. Create a user with email (username auto-generated)
    print("\n1. Creating User with email (username auto-generated)...")
    user = User.objects.create_user(
        email='demo.user@example.com',
        password='SecurePass123!',
        first_name='Demo',
        last_name='User',
        phone_number='+12025551234'
    )
    print(f"   ✓ User created: {user.email}")
    print(f"   ✓ Username auto-generated: {user.username}")
    print(f"   ✓ Phone number: {user.phone_number}")
    
    # 2. Check profile auto-creation
    print("\n2. Checking Profile auto-creation (via signals)...")
    print(f"   ✓ Profile exists: {hasattr(user, 'profile')}")
    print(f"   ✓ Profile: {user.profile}")
    
    # 3. Update profile
    print("\n3. Updating Profile information...")
    user.profile.bio = "Demo user for testing refactored models"
    user.profile.occupation = "Software Engineer"
    user.profile.company = "Tech Corp"
    user.profile.id_proof_type = "passport"
    user.profile.id_proof_number = "AB1234567"
    user.profile.save()
    print(f"   ✓ Profile updated: {user.profile.occupation} at {user.profile.company}")
    
    # 4. Add addresses
    print("\n4. Creating multiple addresses...")
    home_address = Address.objects.create(
        user=user,
        address_type='home',
        street_address_1='123 Main Street',
        street_address_2='Apt 4B',
        city='New York',
        state_province='NY',
        postal_code='10001',
        country='US',
        is_default=True
    )
    print(f"   ✓ Home address created: {home_address.get_formatted_address()}")
    
    work_address = Address.objects.create(
        user=user,
        address_type='work',
        street_address_1='456 Business Ave',
        city='New York',
        state_province='NY',
        postal_code='10002',
        country='US'
    )
    print(f"   ✓ Work address created: {work_address.city}, {work_address.state_province}")
    
    # 5. Check default address logic
    print("\n5. Testing default address logic...")
    print(f"   ✓ Home address is default: {home_address.is_default}")
    print(f"   ✓ Work address is default: {work_address.is_default}")
    
    # 6. Phone verification
    print("\n6. Creating phone verification...")
    verification = create_verification_for_user(
        user=user,
        verification_type='phone',
        phone_number=user.phone_number
    )
    print(f"   ✓ Verification created: {verification}")
    print(f"   ✓ OTP code generated: {verification.otp_code} (normally sent via SMS)")
    print(f"   ✓ Status: {verification.status}")
    print(f"   ✓ Expires at: {verification.expires_at}")
    
    # 7. Verify the OTP
    print("\n7. Verifying OTP code...")
    result = verification.verify(verification.otp_code)
    print(f"   ✓ Verification result: {result}")
    print(f"   ✓ Verification status: {verification.status}")
    
    # Refresh user to get updated is_phone_verified
    user.refresh_from_db()
    print(f"   ✓ User phone verified: {user.is_phone_verified}")
    
    # 8. Summary
    print("\n" + "=" * 70)
    print("SUMMARY")
    print("=" * 70)
    print(f"Total Users: {User.objects.count()}")
    print(f"Total Profiles: {Profile.objects.count()}")
    print(f"Total Addresses: {Address.objects.count()}")
    print(f"Total Verifications: {Verification.objects.count()}")
    
    print("\n✅ All models and services working correctly!")
    print("=" * 70)
    
    # Cleanup
    print("\n8. Cleaning up demo data...")
    user.delete()
    print("   ✓ Demo data cleaned up")


if __name__ == '__main__':
    try:
        demo()
    except Exception as e:
        print(f"\n❌ Error during demo: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)
