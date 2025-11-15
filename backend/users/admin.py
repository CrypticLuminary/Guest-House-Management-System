"""
Admin configuration for user-related models.
"""
from django.contrib import admin
from django.contrib.auth.admin import UserAdmin as BaseUserAdmin
from django.utils.translation import gettext_lazy as _

from .models import User, Profile, Address, Verification


@admin.register(User)
class UserAdmin(BaseUserAdmin):
    """Admin interface for User model."""
    
    list_display = (
        'username', 'email', 'first_name', 'last_name',
        'is_phone_verified', 'is_staff', 'date_joined'
    )
    list_filter = (
        'is_staff', 'is_superuser', 'is_active',
        'is_phone_verified', 'date_joined'
    )
    search_fields = ('username', 'first_name', 'last_name', 'email', 'phone_number')
    ordering = ('-date_joined',)
    
    fieldsets = (
        (None, {'fields': ('username', 'password')}),
        (_('Personal info'), {
            'fields': (
                'first_name', 'last_name', 'email',
                'phone_number', 'date_of_birth', 'avatar'
            )
        }),
        (_('Permissions'), {
            'fields': (
                'is_active', 'is_staff', 'is_superuser',
                'groups', 'user_permissions'
            ),
        }),
        (_('Verification'), {
            'fields': ('is_phone_verified',)
        }),
        (_('Important dates'), {
            'fields': ('last_login', 'date_joined', 'created_at', 'updated_at')
        }),
    )
    
    readonly_fields = ('created_at', 'updated_at', 'date_joined', 'last_login')


@admin.register(Profile)
class ProfileAdmin(admin.ModelAdmin):
    """Admin interface for Profile model."""
    
    list_display = (
        'user', 'occupation', 'company', 'id_proof_type', 'created_at'
    )
    list_filter = ('id_proof_type', 'created_at')
    search_fields = (
        'user__username', 'user__email', 'user__first_name',
        'user__last_name', 'occupation', 'company'
    )
    readonly_fields = ('created_at', 'updated_at')
    
    fieldsets = (
        (_('User'), {
            'fields': ('user',)
        }),
        (_('Professional Info'), {
            'fields': ('bio', 'occupation', 'company', 'website')
        }),
        (_('Identification'), {
            'fields': ('id_proof_type', 'id_proof_number')
        }),
        (_('Emergency Contact'), {
            'fields': ('emergency_contact_name', 'emergency_contact_phone')
        }),
        (_('Preferences'), {
            'fields': ('preferences',)
        }),
        (_('Metadata'), {
            'fields': ('created_at', 'updated_at'),
            'classes': ('collapse',)
        }),
    )


@admin.register(Address)
class AddressAdmin(admin.ModelAdmin):
    """Admin interface for Address model."""
    
    list_display = (
        'user', 'address_type', 'city', 'state_province',
        'country', 'is_default', 'created_at'
    )
    list_filter = ('address_type', 'country', 'is_default', 'created_at')
    search_fields = (
        'user__username', 'user__email', 'street_address_1',
        'street_address_2', 'city', 'state_province', 'postal_code'
    )
    readonly_fields = ('created_at', 'updated_at')
    
    fieldsets = (
        (_('User'), {
            'fields': ('user', 'address_type', 'is_default')
        }),
        (_('Address Details'), {
            'fields': (
                'street_address_1', 'street_address_2',
                'city', 'state_province', 'postal_code', 'country'
            )
        }),
        (_('Metadata'), {
            'fields': ('created_at', 'updated_at'),
            'classes': ('collapse',)
        }),
    )


@admin.register(Verification)
class VerificationAdmin(admin.ModelAdmin):
    """Admin interface for Verification model."""
    
    list_display = (
        'user', 'verification_type', 'status', 'phone_number',
        'attempts', 'max_attempts', 'expires_at', 'created_at'
    )
    list_filter = ('verification_type', 'status', 'created_at')
    search_fields = (
        'user__username', 'user__email', 'phone_number', 'email'
    )
    readonly_fields = ('created_at', 'updated_at', 'verified_at')
    
    fieldsets = (
        (_('User & Type'), {
            'fields': ('user', 'verification_type', 'status')
        }),
        (_('Contact Info'), {
            'fields': ('phone_number', 'email')
        }),
        (_('Verification Details'), {
            'fields': ('otp_code', 'attempts', 'max_attempts', 'expires_at')
        }),
        (_('Timestamps'), {
            'fields': ('created_at', 'updated_at', 'verified_at'),
            'classes': ('collapse',)
        }),
    )
    
    def has_add_permission(self, request):
        """Disable manual creation through admin."""
        return False
