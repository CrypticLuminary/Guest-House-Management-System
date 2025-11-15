"""
Users app configuration.
"""
from django.apps import AppConfig


class UsersConfig(AppConfig):
    """Configuration for the users app."""
    
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'users'
    verbose_name = 'User Management'

    def ready(self):
        """
        Import signal handlers when the app is ready.
        
        This ensures signals are connected when Django starts.
        """
        import users.signals  # noqa: F401
