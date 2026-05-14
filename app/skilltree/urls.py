from django.urls import path
from . import views

urlpatterns = [
    path('', view=views.tree_view, name='tree'),
    path('habits/', view=views.habits_view, name='habits'),
    path('history/', view=views.history_view, name='history'),
    path('action/', view=views.action_view, name='action')
]
