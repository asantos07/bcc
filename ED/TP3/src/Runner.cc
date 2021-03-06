//
// Created by Ariel Santos on 24/03/21.
//

#include <iostream>
#include <random>

#include "Queue.hh"
#include "Runner.hh"

void Runner::run_command(CommandType type, const std::string &text) {
  switch (type) {
	case CommandType::Load: load(text);
	  break;
	case CommandType::Decode: decode(text);
	  break;
	case CommandType::Encode: encode(text);
	  break;
  }
}

void Runner::load(const std::string &text) {
  parse_tree = BTree(text.front());

  for (auto letter_it = text.begin() + 1; letter_it != text.end(); ++letter_it)
	parse_tree.add_leaf(*letter_it);
}

void Runner::decode(const std::string &text) {
  Queue coords;

  // é necessário ignorar o primeiro 'x', já que não temos nenhum caminho antes
  for (auto c_it = text.begin() + 1; c_it != text.end(); ++c_it) {
	if ((*c_it) == 'x') {
	  std::cout << parse_tree.get_node(coords);
	  coords = Queue();
	  continue;
	}
	coords.enqueue(*c_it);
  }

  std::cout << parse_tree.get_node(coords) << '\n';
}

void Runner::encode(const std::string &text) {
  std::random_device dev;
  std::default_random_engine prng(dev());
  std::uniform_int_distribution<std::default_random_engine::result_type> dist(1, 5);

  for (char c : text) {
	Queue path = parse_tree.get_path(c), cypher;

	while (!path.empty()) {
	  char coord = path.dequeue();
	  auto rand = dist(prng) * 2; // sempre par

	  // +48 para bater com cód ASCII para '0'
	  if (coord == -1)
		cypher.enqueue(rand - 1 + 48); // ímpar
	  else if (coord == 1)
		cypher.enqueue(rand - 2 + 48); // par
	}

	std::cout << 'x' << cypher;
  }
  std::cout << '\n';
}
