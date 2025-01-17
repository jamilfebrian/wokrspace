import 'package:faker/faker.dart';
import 'package:flutter/material.dart';

void main() => runApp(const MyApp());

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      debugShowCheckedModeBanner: false,
      home: MyWidget(),
    );
  }
}

class MyWidget extends StatefulWidget {
  const MyWidget({super.key});

  @override
  State<MyWidget> createState() => _MyWidgetState();
}

class _MyWidgetState extends State<MyWidget> {
  var faker = Faker();
  final List<Map<String, dynamic>> myList = [
    {
      "Name": "Jamil",
      "Age": 24,
      "Food": ["A", "B", "C"],
      "isStudent": true,
    },
    {
      "Name": "Doni",
      "Age": 25,
      "Food": ["D", "E", "F"],
      "isStudent": false
    },
    {
      "Name": "Okky",
      "Age": 23,
      "Food": ["X", "Y", "Z"],
      "isStudent": true
    },
    {
      "Name": "Aini",
      "Age": 23,
      "Food": ["X", "Y", "Z"],
      "isStudent": true
    },
  ];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          title: const Text("Wassap"),
        ),
        body: ListView(
          children: myList.map((data) {
            return ListTile(
              leading: const CircleAvatar(),
              title: Text(data["Name"]!),
              subtitle: Text(faker.lorem.sentence()),
              trailing: const Text("20:33"),
            );
          }).toList(),
        ));
  }
}
