from django.urls import path
from . import views

from django.http import HttpResponse

def index(request):
    return HttpResponse("Hallo")

urlpatterns = [
    path('members/', views.members, name='members'),
    path('', index),
]