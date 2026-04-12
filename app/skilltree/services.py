from rest_framework.response import Response
from rest_framework import status
from django.db import transaction
from .models import Profile


def registry_service(serializer):
    if serializer.is_valid():
        try:
            with transaction.atomic():
                user = serializer.save()
                Profile.objects.create(
                    user   = user,
                    level  = 1,
                    streak = 0
                )
            return Response(
                {
                "message": "User and profile created successfully.",
                "user_id": user.pk
                },
                status=status.HTTP_201_CREATED
            )
        except Exception as ex:
            return Response({"error": str(ex)}, status=status.HTTP_400_BAD_REQUEST)
    
    return Response({"error": str(ex)}, status=status.HTTP_400_BAD_REQUEST)
                