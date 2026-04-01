#ifndef GAME_H
#define GAME_H

#include <string>
#include <stdexcept>
#include <cstddef>


struct Node;
struct GameState;
class Card;
class Pile;

/**
 * Skill card type
 * MULTIPLIER: Increases the score multiplier for the next score card
 * HEAD_BUTT: Moves the card at the bottom (tail) of the discard pile to the top (head) of the draw pile
 */
enum SkillCardType {
    MULTIPLIER,
    HEAD_BUTT,
};

/** 
 * Card base class
 * TODO: Add member variables and functions if needed
 */
class Card {
public:
    explicit Card() = default;
    virtual ~Card() = default;
    // Execute card effect, implemented by derived classes. You need to implement the specific effect of the current card type in each derived class
    virtual void play(GameState& game_state) = 0;
};

/** 
 * Score card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class ScoreCard: public Card {
private:
    int point_{};
public:
    explicit ScoreCard(int point);
    void play(GameState &game_state) override;
    ~ScoreCard() override = default;
};

/** 
 * Skill card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class SkillCard: public Card {
private:
    SkillCardType type_{};
public:
    explicit SkillCard(SkillCardType skill_card_type);
    void play(GameState &game_state) override;
    ~SkillCard() override = default;
};

/**
 * Power card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class PowerCard: public Card {
private:
    int plus_count_{};
public:
    explicit PowerCard(int plus_count);
    void play(GameState &game_state) override;
    ~PowerCard() override = default;
};

/** 
 * Linked list node structure
 * TODO: Add or modify member variables and functions as needed
 */
struct Node {
    Card* card{nullptr};
    Node* next{nullptr};
    explicit Node(Card* c=nullptr): card(c), next(nullptr) {}
};

/** 
 * Pile class
 * All three piles (draw pile, discard pile, hand pile) are represented using this class
 * TODO: Modify as needed, add member variables and functions
 */
class Pile {
private:
    int size_;
    Node* head_{nullptr};
    Node* tail_{nullptr};

public:
    friend void outShuffle(GameState&);
    friend void inShuffle(GameState&);
    friend void oddEvenShuffle(GameState&);
    friend class GameController;

    // Constructor and destructor declarations
    // Please implement them in the later part of the file
    Pile();
    ~Pile();

    // Return pile size
    int size() const {
        return size_;
    }

    // Check if pile is empty
    bool empty() const {
        return size_ == 0;
    }

    // Add a card to the end of the pile
    // This function is used to initialize the pile in main.cpp
    // Please implement this function in the later part of the file
    void appendCard(Card* card);
    // Insert an existing node at head
    void pushHead(Node* n) {
        if (!n) return;
        n->next = head_;
        head_ = n;
        if (!tail_) tail_ = n;
        ++size_;
    }
    // Remove tail and return node
    Node* popTail() {
        if (!tail_) return nullptr;
        if (head_ == tail_) {
            Node* n = tail_;
            head_ = tail_ = nullptr;
            --size_;
            n->next = nullptr;
            return n;
        }
        Node* prev = head_;
        while (prev->next != tail_) prev = prev->next;
        Node* n = tail_;
        prev->next = nullptr;
        tail_ = prev;
        --size_;
        n->next = nullptr;
        return n;
    }
    // Pop from head, return node pointer (caller owns Node* and should delete it or re-link)
    Node* popHead() {
        if (!head_) return nullptr;
        Node* n = head_;
        head_ = head_->next;
        if (!head_) tail_ = nullptr;
        n->next = nullptr;
        --size_;
        return n;
    }
    // Append an existing node at tail (does not change node->next)
    void appendNode(Node* n) {
        if (!n) return;
        n->next = nullptr;
        if (!tail_) {
            head_ = tail_ = n;
        } else {
            tail_->next = n;
            tail_ = n;
        }
        ++size_;
    }
    // Remove and return k-th node (1-based) from this pile; returns pair(node, prev)
    Node* removeKth(int k) {
        if (k < 1 || k > size_) return nullptr;
        Node* prev = nullptr;
        Node* cur = head_;
        for (int i = 1; i < k; ++i) {
            prev = cur;
            cur = cur->next;
        }
        if (prev) prev->next = cur->next; else head_ = cur->next;
        if (cur == tail_) tail_ = prev;
        cur->next = nullptr;
        --size_;
        return cur;
    }
    // Move all nodes from this pile to another (append order preserved)
    void moveAllTo(Pile& dst) {
        while (head_) {
            Node* n = popHead();
            dst.appendNode(n);
        }
    }
};

/**
 * Game state structure
 * Stores current game state information such as discard pile, draw pile, hand pile status, total score, current multiplier, etc.
 * TODO: Add member variables and functions as needed
 */
struct GameState {
    Pile hand_{};            // Hand pile
    Pile draw_pile_{};       // Draw pile
    Pile discard_pile_{};    // Discard pile
    long long total_score{0};
    long long power_add{0};
    int multiplier{1};
};

/** 
 * Shuffle function declarations
 * outShuffle: Shuffle the discard pile into the draw pile in order
 * inShuffle: Shuffle the discard pile into the draw pile in reverse order
 * oddEvenShuffle: Separate the discard pile by odd and even positions, with odd positions first and even positions last
 * Please implement these functions in the later part of this file
*/
void outShuffle(GameState &game_state);
void inShuffle(GameState &game_state);
void oddEvenShuffle(GameState &game_state);

/**
 * Game controller class
 * Used to handle game logic
 */
class GameController {
private:
    GameState game_state_;
    // Shuffle function pointer
    void (*shuffle_)(GameState&);
public:
    explicit GameController(int mode);

    // Draw 5 cards from the draw pile to the hand pile
    // This is a function declaration, please implement this function in the later part of the file
    void draw();

    // Play the card_to_play-th card in the hand pile (counting from 1)
    // This is a function declaration, please implement this function in the later part of the file
    void play(int card_to_play);

    // End the current turn, move all cards in the hand pile to the discard pile
    // This is a function declaration, please implement this function in the later part of the file
    void finish();

    // Shuffle in the specified manner
    // This is a function declaration, please implement this function in the later part of the file
    void shuffle();

    // Query current total score
    // This is a function declaration, please implement this function in the later part of the file
    int queryScore();

    // Query draw pile size
    int queryDrawPileSize() const {
        return game_state_.draw_pile_.size();
    }

    // Query hand pile size
    int queryHandSize() const {
        return game_state_.hand_.size();
    }

    // Query discard pile size
    int queryDiscardPileSize() const {
        return game_state_.discard_pile_.size();
    }

    // Get reference to the draw pile
    // Used to load cards in main.cpp
    Pile& drawPile() {
        return game_state_.draw_pile_;
    }
};

// ================= 洗牌函数实现 ===================

/**
 * Shuffle the discard pile into the draw pile in order
 * TODO: Implement this function
 */
void outShuffle(GameState& game_state) {
    // append discard to draw in order
    game_state.discard_pile_.moveAllTo(game_state.draw_pile_);
}

/**
 * Shuffle the discard pile into the draw pile in reverse order
 * TODO: Implement this function
 */
void inShuffle(GameState& game_state) {
    // reverse discard order into draw tail
    // We can collect nodes and push back in reverse by rebuilding list
    // Approach: pop from discard and push to a temporary vector, then append back reversed
    // But without vector to keep memory simple, we can reverse the linked list then moveAllTo
    Node* prev = nullptr;
    Node* cur = game_state.discard_pile_.head_;
    while (cur) {
        Node* nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    // Now prev is new head of reversed list
    game_state.discard_pile_.head_ = prev;
    // Fix tail and size unchanged
    // Recompute tail
    game_state.discard_pile_.tail_ = nullptr;
    Node* tcur = game_state.discard_pile_.head_;
    while (tcur && tcur->next) tcur = tcur->next;
    if (tcur) game_state.discard_pile_.tail_ = tcur;
    game_state.discard_pile_.moveAllTo(game_state.draw_pile_);
}

/**
 * Separate the discard pile by odd and even positions, with odd positions first and even positions last, and shuffle into the draw pile
 * TODO: Implement this function
 */
void oddEvenShuffle(GameState& game_state) {
    // Split discard by positions into odd then even, preserving order
    Pile odd;
    Pile even;
    int idx = 1;
    while (!game_state.discard_pile_.empty()) {
        Node* n = game_state.discard_pile_.popHead();
        if (idx % 2 == 1) odd.appendNode(n); else even.appendNode(n);
        ++idx;
    }
    odd.moveAllTo(game_state.draw_pile_);
    even.moveAllTo(game_state.draw_pile_);
}
    
// ======================================================




// ================== Game State Structure Implementation ===================
// If you added functions to the GameState structure, you can implement them here





// ======================================================




// ================= Card Class Implementation ===========================

// === Score Card Class Implementation ===
ScoreCard::ScoreCard(int point) : point_(point) {}

void ScoreCard::play(GameState &game_state) {
   long long gained = (static_cast<long long>(point_) + game_state.power_add) * game_state.multiplier;
   game_state.total_score += gained;
   game_state.multiplier = 1;
}
// ===================

// === Skill Card Class Implementation ===
SkillCard::SkillCard(SkillCardType skill_card_type) : type_(skill_card_type) {}
void SkillCard::play(GameState &game_state) {
    if (type_ == SkillCardType::MULTIPLIER) {
        game_state.multiplier += 1;
    } else if (type_ == SkillCardType::HEAD_BUTT) {
        // Move discard tail to draw head
        Node* n = game_state.discard_pile_.popTail();
        if (n) {
            game_state.draw_pile_.pushHead(n);
        }
    }
}
// ===================

// === Power Card Class Implementation ===

// === Power Card Class Implementation ===
PowerCard::PowerCard(int plus_count) : plus_count_(plus_count) {}
void PowerCard::play(GameState &game_state) {
    game_state.power_add += plus_count_;
}
// ===================

// ======================================================




// ================= Pile Class Implementation ===========================
Pile::Pile() {
    size_ = 0;
    head_ = tail_ = nullptr;
}
Pile::~Pile() {
    // delete all remaining nodes and owned cards
    while (head_) {
        Node* n = head_;
        head_ = head_->next;
        delete n->card;
        delete n;
    }
    tail_ = nullptr;
    size_ = 0;
}

/**
  * Add a card to the end of the pile
  * TODO: Implement this function
  */
void Pile::appendCard(Card* card) {
    Node* n = new Node(card);
    appendNode(n);
}

// TODO: Implement the functions you declared

// ======================================================




// ================= Game Controller Class Implementation ======================

/**
 * Constructor, initialize game state and shuffle mode
 * @param mode Shuffle mode, 1 for outShuffle, 2 for inShuffle, 3 for oddEvenShuffle
 * TODO: Implement this function
 */
GameController::GameController(int mode){
    if (mode == 1) shuffle_ = &outShuffle;
    else if (mode == 2) shuffle_ = &inShuffle;
    else if (mode == 3) shuffle_ = &oddEvenShuffle;
    else throw std::runtime_error("Invalid shuffle mode");
}
/**
 * Draw 5 cards from the draw pile to the hand pile
 * TODO: Implement this function
 */
void GameController::draw() {
    for (int i = 0; i < 5; ++i) {
        if (game_state_.draw_pile_.empty()) {
            if (!game_state_.discard_pile_.empty()) {
                shuffle_(game_state_);
            } else {
                break;
            }
        }
        if (game_state_.draw_pile_.empty()) break;
        Node* n = game_state_.draw_pile_.popHead();
        game_state_.hand_.appendNode(n);
    }
}

/**
 * Play the card_to_play-th card in the hand pile (counting from 1)
 * TODO: Implement this function
 * Reference implementation approach:
    * 1. Find the corresponding hand card node
    * 2. Remove the card from the hand pile
    * 3. Execute the card's effect
    * 4. If it's a score card or skill card, move it to the discard pile; if it's a power card, delete it
 */
void GameController::play(int card_to_play) {
    if (card_to_play < 1 || card_to_play > game_state_.hand_.size_) {
        throw std::runtime_error("Invalid Operation");
    }
    Node* n = game_state_.hand_.removeKth(card_to_play);
    if (!n) throw std::runtime_error("Invalid Operation");
    // Determine type via dynamic_cast
    if (auto sc = dynamic_cast<ScoreCard*>(n->card)) {
        sc->play(game_state_);
        // move to discard
        game_state_.discard_pile_.appendNode(n);
    } else if (auto sk = dynamic_cast<SkillCard*>(n->card)) {
        sk->play(game_state_);
        game_state_.discard_pile_.appendNode(n);
    } else if (auto pw = dynamic_cast<PowerCard*>(n->card)) {
        pw->play(game_state_);
        // destroy card node
        delete n->card;
        delete n;
    } else {
        // unknown type
        delete n->card;
        delete n;
        throw std::runtime_error("Invalid Operation");
    }
}

/**
 * Shuffle in the specified manner
 * TODO: Implement this function
 */
void GameController::shuffle() {
    shuffle_(game_state_);
}

/**
 * End the current turn, move all cards in the hand pile to the discard pile
 * TODO: Implement this function
 */
void GameController::finish() {
    game_state_.hand_.moveAllTo(game_state_.discard_pile_);
}

/**
 * Query current total score
 * TODO: Implement this function
 */
int GameController::queryScore() {
    return static_cast<int>(game_state_.total_score);
}

#endif //GAME_H
