from django.urls import path
from . import views

urlpatterns = [
    path('register/', views.register_view, name='register'),
    path('login/', views.login_view, name='login'),
    path('tree/', views.tree_view, name='tree'),
    path('habits', views.habits_view, name='habits'),
]
