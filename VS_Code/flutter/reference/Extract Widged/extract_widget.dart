import 'package:faker/faker.dart';
import 'package:flutter/material.dart';

void main() => runApp(const ScaffoldExampleApp());

class ScaffoldExampleApp extends StatelessWidget {
  const ScaffoldExampleApp({super.key});

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      debugShowCheckedModeBanner: false,
      home: ScaffoldExample(),
    );
  }
}

class ScaffoldExample extends StatefulWidget {
  const ScaffoldExample({super.key});

  @override
  State<ScaffoldExample> createState() => _ScaffoldExampleState();
}

class _ScaffoldExampleState extends State<ScaffoldExample> {
  var faker = Faker();

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          title: const Text('Wassapp'),
          backgroundColor: Colors.blue,
        ),
        backgroundColor: Colors.white,
        body: SafeArea(
            child: ListView.builder(
          itemCount: 20,
          itemBuilder: (context, index) {
            return ChatItem(
              imageURL: "https://picsum.photos/id/$index/200/300",
              tittle: faker.person.name(),
              subtitle: faker.lorem.sentence(),
            );
          },
        )));
  }
}

class ChatItem extends StatelessWidget {
  final String? imageURL;
  final String? tittle;
  final String? subtitle;
  const ChatItem({super.key, this.imageURL, this.tittle, this.subtitle});

  @override
  Widget build(BuildContext context) {
    return ListTile(
      leading: CircleAvatar(
        backgroundImage: NetworkImage(imageURL!),
      ),
      title: Text(tittle!),
      subtitle: Text(subtitle!),
      trailing: const Text("20:33"),
    );
  }
}
