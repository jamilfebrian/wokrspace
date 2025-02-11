import 'package:flutter/material.dart';
import 'package:home_automation/data/notifiers.dart';
import 'package:home_automation/providers/products_provider.dart';
import 'package:provider/provider.dart';

import 'pages/room_pages.dart';
import 'widgets_tree.dart';

void main() {
  runApp(const MainApp());
}

class MainApp extends StatelessWidget {
  const MainApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MultiProvider(
        providers: [
          ChangeNotifierProvider(
            create: (context) => Products(),
          ),
        ],
        child: ValueListenableBuilder(
          valueListenable: isDarkModeNotifier,
          builder: (context, isDarkMode, child) {
            return MaterialApp(
              debugShowCheckedModeBanner: false,
              theme: ThemeData(
                colorScheme: ColorScheme.fromSeed(
                  seedColor: Colors.teal,
                  brightness: isDarkMode ? Brightness.light : Brightness.dark,
                ),
              ),
              home: const WidgetsTree(),
              routes: {
                RoomPages.routeName: (ctx) => const RoomPages(),
              },
            );
          },
        ));
  }
}
