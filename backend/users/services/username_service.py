"""
Username generation service.

Handles automatic username generation from email or name components.
"""
import re
from django.contrib.auth import get_user_model
from django.utils.text import slugify


def generate_username_from_email(email):
    """
    Generate a username from an email address.
    
    Args:
        email: Email address to derive username from
        
    Returns:
        str: Generated username (local part of email)
    """
    if not email:
        return None
    
    # Extract local part before @
    local_part = email.split('@')[0]
    
    # Clean and slugify
    username = slugify(local_part).replace('-', '_')
    
    # Remove any non-alphanumeric characters except underscore
    username = re.sub(r'[^\w]', '', username)
    
    return username[:150]  # Django username max length


def generate_username_from_name(first_name, last_name):
    """
    Generate a username from first and last name.
    
    Args:
        first_name: User's first name
        last_name: User's last name
        
    Returns:
        str: Generated username
    """
    if not first_name and not last_name:
        return None
    
    # Combine names
    full_name = f"{first_name}_{last_name}".strip('_')
    
    # Clean and slugify
    username = slugify(full_name).replace('-', '_')
    
    # Remove any non-alphanumeric characters except underscore
    username = re.sub(r'[^\w]', '', username)
    
    return username[:150]


def make_unique_username(base_username):
    """
    Make a username unique by appending a number if needed.
    
    Args:
        base_username: The base username to make unique
        
    Returns:
        str: A unique username
    """
    User = get_user_model()
    
    if not base_username:
        base_username = 'user'
    
    username = base_username
    counter = 1
    
    while User.objects.filter(username=username).exists():
        username = f"{base_username}{counter}"
        counter += 1
    
    return username


def ensure_username(email=None, first_name=None, last_name=None):
    """
    Ensure a valid unique username exists.
    
    Tries multiple strategies in order:
    1. Generate from email
    2. Generate from name
    3. Generate generic username
    
    Args:
        email: User's email address
        first_name: User's first name
        last_name: User's last name
        
    Returns:
        str: A valid unique username
    """
    # Try email first
    if email:
        username = generate_username_from_email(email)
        if username:
            return make_unique_username(username)
    
    # Try name
    if first_name or last_name:
        username = generate_username_from_name(first_name, last_name)
        if username:
            return make_unique_username(username)
    
    # Fallback to generic username
    return make_unique_username('user')
