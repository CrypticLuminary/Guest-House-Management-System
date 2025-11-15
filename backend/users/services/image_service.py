"""
Image processing service.

Handles image compression and optimization for user avatars.
"""
import io
from PIL import Image
from django.core.files.uploadedfile import InMemoryUploadedFile


# Maximum file size in bytes (5MB)
MAX_IMAGE_SIZE = 5 * 1024 * 1024

# Target dimensions for avatar images
AVATAR_MAX_WIDTH = 800
AVATAR_MAX_HEIGHT = 800

# Image quality for compression
IMAGE_QUALITY = 85


def compress_image(image_file, max_width=AVATAR_MAX_WIDTH, max_height=AVATAR_MAX_HEIGHT, quality=IMAGE_QUALITY):
    """
    Compress and resize an image file.
    
    Args:
        image_file: Django file object containing the image
        max_width: Maximum width in pixels
        max_height: Maximum height in pixels
        quality: JPEG quality (1-100)
        
    Returns:
        InMemoryUploadedFile: Compressed image file
    """
    try:
        # Open image
        img = Image.open(image_file)
        
        # Convert RGBA to RGB if necessary
        if img.mode in ('RGBA', 'LA', 'P'):
            # Create white background
            background = Image.new('RGB', img.size, (255, 255, 255))
            if img.mode == 'P':
                img = img.convert('RGBA')
            background.paste(img, mask=img.split()[-1] if img.mode == 'RGBA' else None)
            img = background
        
        # Calculate new dimensions maintaining aspect ratio
        img.thumbnail((max_width, max_height), Image.Resampling.LANCZOS)
        
        # Save to BytesIO
        output = io.BytesIO()
        
        # Determine format
        format_type = 'JPEG'
        if hasattr(image_file, 'name'):
            if image_file.name.lower().endswith('.png'):
                format_type = 'PNG'
        
        img.save(output, format=format_type, quality=quality, optimize=True)
        output.seek(0)
        
        # Create new InMemoryUploadedFile
        return InMemoryUploadedFile(
            output,
            'ImageField',
            image_file.name,
            f'image/{format_type.lower()}',
            output.getbuffer().nbytes,
            None
        )
    except Exception as e:
        # If compression fails, return original file
        # Log the error in production
        print(f"Image compression failed: {e}")
        return image_file


def compress_user_avatar(avatar_field):
    """
    Compress a user's avatar image if needed.
    
    Args:
        avatar_field: ImageField from User model
    """
    if not avatar_field:
        return
    
    try:
        # Check if file size exceeds threshold
        if hasattr(avatar_field, 'size') and avatar_field.size > MAX_IMAGE_SIZE:
            # Compress the image
            compressed = compress_image(avatar_field.file)
            
            # Replace the field's file with compressed version
            avatar_field.file = compressed
            
    except Exception as e:
        # If compression fails, log but don't block the save
        print(f"Avatar compression failed: {e}")


def validate_image_file(image_file):
    """
    Validate an image file for security and format.
    
    Args:
        image_file: File to validate
        
    Returns:
        tuple: (is_valid: bool, error_message: str or None)
    """
    if not image_file:
        return True, None
    
    # Check file size
    if hasattr(image_file, 'size') and image_file.size > MAX_IMAGE_SIZE:
        return False, f"Image file too large. Maximum size is {MAX_IMAGE_SIZE / 1024 / 1024}MB"
    
    # Try to open and verify it's a valid image
    try:
        img = Image.open(image_file)
        img.verify()
        return True, None
    except Exception as e:
        return False, f"Invalid image file: {str(e)}"
