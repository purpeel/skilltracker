from rest_framework.response import Response
from rest_framework.permissions import AllowAny
from rest_framework.authtoken.models import Token
from rest_framework.decorators import api_view, permission_classes
from .models import Tree, Node
from .serializers import TreeSerializer, NodeSerializer, RegistrySerializer
from .services import registry_service
from django.contrib.auth import authenticate


@api_view(['GET'])
def tree_view(request):
    trees = Tree.objects.filter( profile__user=request.user ).prefetch_related( 'nodes' )
    
    serializer = TreeSerializer( trees, many=True )
    
    return Response( serializer.data )    


@api_view(['GET'])
def habits_view(request):
    nodes = Node.objects.filter( tree__profile__user=request.user, node_type="H" )
    
    serializer = NodeSerializer( nodes, many=True )
    
    return Response( serializer.data )


@api_view(['POST'])
@permission_classes([AllowAny])
def login_view(request):
    username = request.data.get('username')
    password = request.data.get('password')
    
    user = authenticate( username=username, password=password )
    
    if user:
        token, created = Token.objects.get_or_create( user=user )
        return Response({
            'token': token.key,
            'user_id': user.pk
        })
    else:
        return Response({'error': 'Invalid login data'}, status=400)


@api_view(['POST'])
@permission_classes([AllowAny])
def register_view(request):
    serializer = RegistrySerializer(data = request.data)
    
    return registry_service(serializer)